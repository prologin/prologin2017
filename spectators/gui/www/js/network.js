let socket, populated = false;

function connect() {
  console.log('trying to connect');
  socket = new WebSocket('ws://' + window.location.host + '/ws');
  socket.onopen = () => {
    backoff = 1;
    console.log('connected to server');
    send('hello');
  };
  socket.onerror = (e) => {
    console.warn(e);
    // check error code, don't reconnect if not needed
    backoff = Math.min(4, backoff + 1);
    setTimeout(connect, 1000 * Math.pow(1.5, backoff));
  };
  socket.onmessage = (msg) => {
    msg = JSON.parse(msg.data);
    console.debug('ws >', msg);
    if (msg.c === 'whatsup') {
      turnCache = [];
      tvMode = msg.tvMode;
      if (tvMode)
        TURN_DURATION /= 2;
      initSamples(msg.firstTurn);
    } else if (msg.c === 'end') {
      // TODO: show game end
    } else if (msg.c === 'turn') {
      let turnData = msg.state;
      let [a, b] = turnData.turn;
      turnIndex = a;
      lastTurn = b;
      if (!populated) {
        // first time we got turn data (not necessarily the first turn)
        populated = true;
        const ANIMATE_SAVE = ANIMATE;
        ANIMATE = false;
        objEach(turnData.players, (pid, player, playerIdx) => {
          player.bench.forEach((apiMaterial, idx) => {
            const material = API_MATERIAL[apiMaterial];
            if (material === null)
              return;
            let tile = boardTiles[playerIdx][idx] = new Tile(material);
            tile.addToBoard(boards[playerIdx], indexToPosition(idx));
          });
        });
        ANIMATE = ANIMATE_SAVE;
      }
      turnCache[turnIndex] = turnData;
      renderTurn(turnIndex);
    }
  };
}

function send(cmd, data) {
  let msg = $.extend({c: cmd}, data);
  console.debug('ws <', msg);
  socket.send(JSON.stringify(msg));
}
