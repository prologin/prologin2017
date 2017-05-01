import asyncio
import json
import logging
import os.path
import random
import sys
import threading
import subprocess

from aiohttp import web
import aiohttp

from state import Reader

logger = logging.Logger(__name__)
root = os.path.abspath(os.path.dirname(__file__))


class Server:
    def __init__(self, state_reader: Reader, tv_mode=False):
        self.state_reader = state_reader
        self.tv_mode = tv_mode

        self.loop = asyncio.get_event_loop()
        self.app = web.Application(loop=self.loop)
        self.app.on_shutdown.append(self.on_shutdown)
        self.app.router.add_static('/www', os.path.join(root, 'www'))
        self.app.router.add_route('GET', '/ws', self.websocket_handler)
        self.ws = web.WebSocketResponse()
        self.state = None

        self.run_thread = threading.Thread(target=self.run_forever)

    async def websocket_handler(self, request):
        if self.ws.prepared:
            logger.warning("session was already opened")
            await self.ws.close(code=998, message="session already opened")
        else:
            logger.info("new WS session")

        self.ws = web.WebSocketResponse()

        await self.ws.prepare(request)
        async for msg in self.ws:
            if msg.type == aiohttp.WSMsgType.TEXT:
                self.handle_message(msg)

        return self.ws

    async def on_shutdown(self, *args):
        if self.ws.prepared and not self.ws.closed:
            await self.ws.close(code=999, message="Server shutdown")

    def send(self, cmd, **kwargs):
        msg = {}
        msg.update(kwargs)
        msg['c'] = cmd
        self.ws.send_str(json.dumps(msg))

    def handle_message(self, msg):
        data = json.loads(msg.data)
        cmd = data['c']
        logging.debug("received WS message: %s", data)
        if cmd == 'hello':
            first_turn = ["PLOMB", "FER"]
            self.send('whatsup',
                      firstTurn=first_turn,
                      canSeek=self.state_reader.can_seek(),
                      canGoBackwards=self.state_reader.can_go_backwards(),
                      tvMode=self.tv_mode)
            self.go_next()
            return
        if cmd == 'next':
            self.go_next()
            return
        if cmd == 'previous':
            self.go_previous()
            return
        if cmd == 'seek':
            self.go_seek(data['index'])
            return

    def start(self):
        self.run_thread.start()

    def go_next(self):
        self.state = self.state_reader.get_next_state()
        if self.state:
            self.send('turn', state=self.state)
            self.state_reader.go_next()
        if self.tv_mode and self.state_reader.is_ended():
            self.end_game()

    def go_previous(self):
        self.state_reader.go_previous()
        self.state = self.state_reader.get_next_state()
        if self.state:
            self.send('turn', state=self.state)

    def go_seek(self, index):
        self.state_reader.go_seek(index)
        self.state = self.state_reader.get_next_state()
        if self.state:
            self.send('turn', state=self.state)

    def run_forever(self):
        # asyncio.set_event_loop(self.loop)
        handler = self.app.make_handler()
        host = '127.0.0.1'
        port = random.randint(9000, 9999)
        while True:
            try:
                srv = self.loop.run_until_complete(
                    self.loop.create_server(handler, host, port))
                break
            except OSError:
                logger.warning("Port %d not available", port)
                port += 1
        url = 'http://{}:{}/www/index.html'.format(host, port)
        for prog in ('chromium', 'google-chrome', 'chromium-browser'):
            try:
                subprocess.check_call([prog, '--app=' + url])
                # browser = asyncio.create_subprocess_exec(prog, '--app=' + url)
                # logging.info("Opening %s with %s", url, prog)
                # self.loop.run_until_complete(browser)
                break
            except FileNotFoundError:
                pass
        try:
            self.loop.run_forever()
        except KeyboardInterrupt:
            pass
        finally:
            self.loop.run_until_complete(srv.wait_closed())
            self.loop.run_until_complete(self.app.shutdown())
            self.loop.run_until_complete(handler.finish_connections(4))
            self.loop.run_until_complete(self.app.cleanup())

    def kill(self):
        self.loop.stop()
        self.loop.call_soon(self.loop.close)
        self.loop.call_soon(sys.exit, 0)

    def end_game(self):
        self.state_reader.wait_end()
        self.send('end')
        self.ws.close(code=1)
        if self.tv_mode:
            self.loop.call_later(1, self.kill)
