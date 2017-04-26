# -*- coding: utf-8 -*-

import ctypes
import json
import threading
import queue
import sys


LIBPROLOGIN = 'libprologin2017.so'


class Reader:
    """
    This class exhibits the reader interface needed by the GUI.
    A Reader object parses the game state from a dump.
    """

    def get_previous_state(self):
        """
        Must only be called in the GUI thread.
        """
        raise NotImplementedError()

    def go_previous(self):
        """
        Prepare the Stechec client to go to the previous turn or raise a
        RuntimeError if this feature is not available.
        Must be called in the GUI thread.
        """
        raise NotImplementedError()

    def go_seek(self, index):
        raise NotImplementedError()

    def get_next_state(self):
        """
        Return if available the next game state.
        Must only be called in the GUI thread.
        """
        raise NotImplementedError()

    def go_next(self):
        """
        Prepare the Stechec client to go to the next turn.
        Must be called in the GUI thread.
        """
        raise NotImplementedError()

    def put_state(self):
        """
        Put the next game state.
        Must be called in the Stechec thread.
        """
        raise NotImplementedError()

    def is_ended(self):
        """
        Return if the game is ended.
        Must be called in the GUI thread.
        """
        raise NotImplementedError()

    def wait_end(self):
        """
        Wait the end of the game.
        Must be called in the GUI thread.
        """
        raise NotImplementedError()

    def do_end(self):
        """
        Make the game end.
        Must be called in the Stechec thread.
        """
        raise NotImplementedError()

    def wait_gui_end(self):
        """
        Wait for the GUI to be done before exiting.
        Must be called in the Stechec thread.
        """
        raise NotImplementedError()

    def do_gui_end(self):
        """
        State that the GUI is done so Stechec can exit.
        Must be called in the GUI thread.
        """
        raise NotImplementedError()

    def get_turn(self):
        """
        Return the turn number.
        Should be called in the GUI thread.
        """
        raise NotImplementedError()

    def build_state(self):
        """
        Build a GameState object from the Stechec’s API and return it.
        Shouldn’t be called out of this class (Stechec thread).
        """
        raise NotImplementedError()

    def can_quit(self):
        raise NotImplementedError()

    def can_go_backwards(self):
        """
        Must return True if the user can browse backwards in time.
        """
        raise NotImplementedError()

    def can_seek(self):
        raise NotImplementedError()


class StechecReader(Reader):
    """
    Stechec reader get the game from the Stechec client.

    It must be used with two separated threads: one for the Stechec client, and
    one for the Gui. The Reader interface must be called only from the GUI
    thread, and the other methods only from the Stechec thread.
    """

    def __init__(self):
        Reader.__init__(self)
        self.end_turn = threading.Event()
        self.end_game = threading.Event()
        self.gui_end = threading.Event()
        self.waiting_end = threading.Event()
        self.released = threading.Event()

        self.waiting_turn = False
        self.pipe = queue.Queue()
        self.turn = 0

        librules = ctypes.cdll.LoadLibrary(LIBPROLOGIN)
        self.get_dump = librules.dump_state_json
        self.get_dump.argtypes = []
        self.get_dump.restype = ctypes.c_char_p

    def can_quit(self):
        return False

    def can_go_backwards(self):
        return False

    def can_seek(self):
        return False

    def go_previous(self):
        raise NotImplementedError("Cannot go backwards in StechecReader")

    def get_next_state(self):
        if self.waiting_turn:
            return None
        game_state = None
        if not self.pipe.empty():
            game_state = self.pipe.get()
            self.waiting_turn = True
            self.turn += 1
        return game_state

    def go_next(self):
        if self.waiting_turn:
            self.waiting_turn = False
            self.end_turn.set()

    def put_state(self):
        if self.waiting_end.is_set():
            self.turn += 1
            # Flushing is used to delay the return of put_state. Yes, this is
            # an ugly hack.
            sys.stdin.flush()
            sys.stdout.flush()
            sys.stderr.flush()
            return
        game_state = self.build_state()
        self.pipe.put(game_state)
        self.end_turn.clear()
        self.end_turn.wait()
        self.end_turn.set()  # wow so fast.

    def is_ended(self):
        return self.end_game.is_set()

    def wait_end(self):
        self.waiting_end.set()
        self.end_turn.set()

    def do_end(self):
        self.end_game.set()

    def wait_gui_end(self):
        self.gui_end.wait()

    def do_gui_end(self):
        self.gui_end.set()

    def get_turn(self):
        return self.turn

    def build_state(self):
        json_dump = self.get_dump().decode()
        return json.loads(json_dump)


class DumpReader(Reader):
    def __init__(self, dump_file):
        self.dump_file = dump_file

        self.turn = -1
        self.turns = [json.loads(line) for line in dump_file]

        self.is_ended_bool = False
        self.go_next()

    def can_quit(self):
        return True

    def can_go_backwards(self):
        return True

    def can_seek(self):
        return True

    def get_next_state(self):
        return self.turns[self.turn]

    def go_next(self):
        self.turn += 1

    def go_previous(self):
        self.turn -= 1

    def go_seek(self, index):
        self.turn = index

    def get_previous_state(self):
        return self.get_next_state()

    def is_ended(self):
        return self.turn == len(self.turns)

    def get_turn(self):
        return self.turn

    def do_gui_end(self):
        pass

    def wait_end(self):
        pass
