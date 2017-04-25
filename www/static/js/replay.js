async function main() {
  window.GEOMETRY_DIR = '/static/geometry';
  window.TEXTURE_DIR = '/static/texture/';

  // load main.js and dependencies
  ['/static/js/lib/three.min.js',
    '/static/js/lib/stats.min.js',
    '/static/js/lib/shaders/ConvolutionShader.js',
    '/static/js/lib/shaders/CopyShader.js',
    '/static/js/lib/shaders/FilmShader.js',
    '/static/js/lib/postprocessing/EffectComposer.js',
    '/static/js/lib/postprocessing/BloomPass.js',
    '/static/js/lib/postprocessing/UnrealBloomPass.js',
    '/static/js/lib/postprocessing/FilmPass.js',
    '/static/js/lib/postprocessing/RenderPass.js',
    '/static/js/lib/postprocessing/MaskPass.js',
    '/static/js/lib/postprocessing/ShaderPass.js',
    '/static/js/lib/OrbitControls.js',
    '/static/js/lib/Tween.js',
    '/static/js/main.js'].forEach(path => {
    $('body').append($('<script/>').attr('src', path));
  });

  let $replay = $('#replay'),
    $replayBoard = $('#replay-board'),
    $playPause = $('#replay-playpause'),
    $previous = $('#replay-previous'),
    $next = $('#replay-next'),
    $turnLabel = $('#replay-turn-label'),
    $turnSlider = $('#replay-turn-slider');

  $('#replay-legend').hide(); // we have our own

  $replayBoard.append($('<div/>').attr('id', 'players'));

  let playing = false, animationEnabled = true;
  let turnIndex = 0, turnForward, playingTimer, nextTurnCallback;

  function render() {
    ANIMATE = turnForward && animationEnabled;
    renderTurn(turnIndex);
  }

  $replay.hide();

  window.END_TURN_CALLBACK = function () {
    if (animationEnabled)
      nextTurnCallback();
    else
      setTimeout(nextTurnCallback, 100);
  };

  await init(document.getElementById('replay-board'));
  initSamples(['PLOMB', 'FER']); // hardcoded from game_state

  console.log('loading dump');

  $.get("dump/")
    .then((game_data) => {
      let turns = turnCache = game_data.split('\n').filter(Boolean).map(JSON.parse);
      lastTurn = turns.length - 1;

      console.log('loaded', turnCache.length, 'turns');

      // ui handlers
      $previous.click(() => {
        $turnSlider.val(turnIndex - 1).trigger('change');
      });

      $next.click(() => {
        $turnSlider.val(turnIndex + 1).trigger('change');
      });

      $turnSlider.change(() => {
        let newTurnIndex = parseInt($turnSlider.val());
        turnForward = newTurnIndex === turnIndex + 1;
        turnIndex = newTurnIndex;
        $previous.prop('disabled', turnIndex <= 0);
        $next.prop('disabled', turnIndex >= turns.length - 1);
        $turnLabel.text(('000' + turns[turnIndex].turn[0]).slice(-3));
        render();
      });
      $turnSlider.attr('min', 0).attr('max', turns.length - 1).val(0);

      nextTurnCallback = function () {
        if (!playing)
          return;
        if (turnIndex >= turns.length - 1) {
          turnIndex = turns.length - 1;
          playing = true; // trigger pause
          $playPause.trigger('click');
          return;
        }
        $turnSlider.val(turnIndex + 1).trigger('change');
      };

      $playPause.click(() => {
        if (!playing && turnIndex >= turns.length - 1)
          return;
        playing = !playing;
        $playPause
          .find('span').text(playing ? 'Pause' : 'Lecture');
        $playPause
          .find('i')
          .toggleClass('fa-play', !playing)
          .toggleClass('fa-pause', playing);
        nextTurnCallback();
      });

      $('body').keydown(e => {
        function stop() {
          e.preventDefault();
          e.stopPropagation();
        }

        const key = e.which, offset = e.shiftKey ? 10 : 1;
        if (key === 32) {
          // space
          stop();
          $playPause.trigger('click');
        } else if (key === 37) {
          // left
          stop();
          $turnSlider.val(turnIndex - offset).trigger('change');
        } else if (key === 39) {
          // right
          stop();
          $turnSlider.val(turnIndex + offset).trigger('change');
        } else if (key === 65) {
          // a
          stop();
          $turnSlider.val(0).trigger('change');
        } else if (key === 69) {
          // e
          stop();
          $turnSlider.val(turns.length - 1).trigger('change');
        } else if (key === 77) {
          // m
          animationEnabled = !animationEnabled;
        }
      });

      // ready, trigger a fake play/pause to init everything
      playing = true;
      $playPause.trigger('click');

      // reveal the UI
      $replay.fadeIn('fast');
    });
}

$(function () {
  (async function () {
    main();
  })();
});
