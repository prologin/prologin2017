'use strict';

let ANIMATE = true;
let INTERACTIVE = false;
let SHOW_FPS = false;

const DIRECTIONS = [{r: 0, c: -1}, {r: 1, c: 0}, {r: 0, c: 1}, {r: -1, c: 0}];
const PLAYERS = [0, 1];
const BOARD_SIZE = 6, H_BOARD_SIZE = BOARD_SIZE / 2;
const TILE_OFF = 1.1, H_TILE_OFF = TILE_OFF / 2, TILE_Y = .33;

let stats, scene, camera, renderer, composer,  cursorSample, $players;
let turnCache = [], turnIndex, tvMode = false, lastTurn = null;
let boards = [], samples = [], catalysts = PLAYERS.map(() => 0);
let lights = {}, books = {};
let boardTiles;
let backoff = 1;
let tick = 0;
let animating = false, playing = true;
let mouse = new THREE.Vector2();
let clock = new THREE.Clock(true);
let raycaster = new THREE.Raycaster();
let domMain;

const FRAGMENT_SHADER = 'uniform float time;uniform float offset;uniform vec2 resolution;uniform float fogDensity;uniform vec3 fogColor;uniform sampler2D texture1;uniform sampler2D texture2;varying vec2 vUv;void main(void){if(fogDensity>1.){gl_FragColor=texture2D(texture2,vUv);return;}vec2 position=-1.0+2.0*vUv;vec4 noise=texture2D(texture1,vUv);vec2 T1=vUv+vec2(1.5,-1.5)*(time+offset)*0.02;vec2 T2=vUv+vec2(-0.5,2.0)*(time+offset)*0.01;T1.x+=noise.x*2.0;T1.y+=noise.y*2.0;T2.x-=noise.y*0.2;T2.y+=noise.z*0.2;float p=texture2D(texture1,T1*2.0).a;vec4 color=texture2D(texture2,T2*2.0);vec4 temp=color*(vec4(p,p,p,p)*2.0)+(color*color-0.1);if(temp.r>1.0){temp.bg+=clamp(temp.r-2.0,0.0,100.0);}if(temp.g>1.0){temp.rb+=temp.g-1.0;}if(temp.b>1.0){temp.rg+=temp.b-1.0;}gl_FragColor=temp;float depth=10.;const float LOG2=1.442695;float fogFactor=exp2(-fogDensity*fogDensity*depth*depth*LOG2);fogFactor=1.0-clamp(fogFactor,0.0,1.0);gl_FragColor=mix(gl_FragColor,vec4(fogColor,gl_FragColor.w),fogFactor);}';
const VERTEX_SHADER = 'uniform vec2 uvScale;varying vec2 vUv;void main(){vUv=uvScale*uv;vec4 mvPosition=modelViewMatrix*vec4(position,1.0);gl_Position=projectionMatrix*mvPosition;}';

const metalUniforms = {
  fogDensity: {value: 0.06},
  fogColor: {value: new THREE.Vector3(0, 0, 0)},
  time: {value: 1.0},
  offset: {value: 0},
  resolution: {value: new THREE.Vector2()},
  uvScale: {value: new THREE.Vector2(1.0, 1.0)},
  texture1: {},
  texture2: {}
};

const TEXTURES = {
  cloud: 'cloud.png',
  book: 'book.jpg',
  bookLogo: 'book-logo.jpg',
  bookNormal: 'book.normal.jpg',
  bookSpecular: 'book.specular.jpg',
  candle: 'candle.jpg',
  candleNormal: 'candle.normal.jpg',
  candleSpecular: 'candle.specular.jpg',
  catalyst: 'catalyst.jpg',
  gold: 'gold.jpg',
  powder: 'powder.jpg',
  norm1: 'norm1.jpg',
  norm2: 'norm2.jpg',
  norm3: 'norm3.jpg',
  wood: 'wood.jpg',
  woodNormal: 'wood.normal.jpg',
  wood2: 'wood2.jpg',
  wood2Normal: 'wood2.normal.jpg',
  wood2Bump: 'wood2.bump.jpg',
  prologin: 'prologin.png',
};

const GEOMETRY_DIR = window.GEOMETRY_DIR || './geometry';
const TEXTURE_DIR = window.TEXTURE_DIR || './texture/';

const GEOMETRIES = {
  tile: new THREE.BoxGeometry(1, .2, 1),
  board: GEOMETRY_DIR + '/board.json',
  book: GEOMETRY_DIR + '/book.json',
  candle: GEOMETRY_DIR + '/candle.json',
  cup: GEOMETRY_DIR + '/cup.json',
  feather: GEOMETRY_DIR + '/feather.json',
  tube: GEOMETRY_DIR + '/tube.json',
  stand: GEOMETRY_DIR + '/stand.json',
};

const API_MATERIAL = {
  VIDE: null,
  PLOMB: 'lead',
  FER: 'iron',
  CUIVRE: 'copper',
  SOUFRE: 'sulfur',
  MERCURE: 'mercury',
};

function boardSign(idx) {
  return idx * 2 - 1;
}

function indexToPosition(idx) {
  return {c: idx % BOARD_SIZE, r: Math.floor(idx / BOARD_SIZE)};
}

function positionToIndex(pos) {
  return pos.r * BOARD_SIZE + pos.c;
}

function objEach(obj, func) {
  Object.keys(obj).forEach((key, idx) => {
    func.call(this, key, obj[key], idx);
  });
}

class Tile {
  constructor(type) {
    console.assert(!!type);
    this.random = Math.random();
    this.disposable = [];
    this.type = undefined;
    this.board = null;
    this.position = null;
    this.material = null;
    this.mesh = new THREE.Mesh(GEOMETRIES.tile, this.material);
    this.mesh.translation = GEOMETRIES.tile.center();
    this.group = new THREE.Group();
    this.group.add(this.mesh);
    this.setType(type);
  }

  setMapOffet(offset) {
    if (this.material.map) {
      this.material.map.offset = offset;
      this.material.needUpdate = true;
    }
    if (this.material.normalMap) {
      this.material.normalMap.offset = offset;
      this.material.needUpdate = true;
    }
  }

  addToBoard(board, position) {
    this.board = board;
    this.position = position;
    board.group.add(this.group);
    this.group.position.set(
      (position.c - BOARD_SIZE / 2 + .5) * TILE_OFF,
      TILE_Y,
      (position.r - BOARD_SIZE / 2 + .5) * TILE_OFF
    );
  }

  morphTo(newType, delay) {
    console.assert(!!this.material);
    this.morphing = true;

    let fadeOut = new TWEEN.Tween(this.material)
      .easing(TWEEN.Easing.Quadratic.In)
      .delay(delay)
      .to({opacity: .8}, 150);
    let fadeIn = new TWEEN.Tween(this.material)
      .easing(TWEEN.Easing.Quadratic.Out)
      .to({opacity: 1}, 150);
    fadeOut.chain(fadeIn).onComplete(() => this.morphing = false).start();

    // let bloomIn = new TWEEN.Tween(bloomPass.copyUniforms.opacity)
    //   .easing(TWEEN.Easing.Exponential.InOut)
    //   .delay(delay)
    //   .to({value: [4, .7]})
    //   .start();

    let melt = new TWEEN.Tween(this.group.scale)
      .easing(TWEEN.Easing.Quadratic.In)
      .delay(delay)
      .to({x: .8, z: .8, y: .5}, 150)
      .onComplete(() => {
        // actually change material
        this.setType(newType);
      });
    let unmelt = new TWEEN.Tween(this.group.scale)
      .easing(TWEEN.Easing.Quadratic.Out)
      .to({x: 1, z: 1, y: 1}, 150);

    if (newType == 'gold' || newType == 'catalyst') {
      let toBag = new TWEEN.Tween(this.group.position)
        .easing(TWEEN.Easing.Quadratic.Out)
        .delay(200)
        .to({y: 12}, 250)
        .onComplete(() => {
          this.destroy();
        });
      unmelt = unmelt.chain(toBag);
    }
    melt.chain(unmelt).start();
  }

  setType(type, delay) {
    console.assert(type);
    if (type === this.type)
      return;
    this.type = type;
    this.group.visible = true;
    this.mesh.material = this.material = this.getMaterial();
    if (ANIMATE) {
      this.group.position.y = 0;
      new TWEEN.Tween(this.group.position)
        .easing(TWEEN.Easing.Quadratic.Out)
        .delay(delay === undefined ? 0 : delay)
        .to({y: TILE_Y}, 300)
        .start();
    }
  }

  transmutesTo() {
    return this.type == 'sulfur' || this.type == 'mercury' ? 'catalyst' : 'gold';
  }

  getMaterial() {
    let that = this;

    function cloneTexture(texture, repeat) {
      let t = texture.clone();
      t.needsUpdate = true;
      if (!repeat) repeat = 1;
      t.repeat.set(repeat, repeat);
      that.disposable.push(t);
      return t;
    }

    function metalMaterial(texture, fog, noise) {
      let u = THREE.UniformsUtils.clone(metalUniforms);
      u.texture1.value = noise || TEXTURES.cloud;
      u.texture2.value = texture;
      u.fogDensity.value = fog;
      u.offset.value = that.random * 10;
      u.uvScale.value = new THREE.Vector2(.8 + Math.random() * 0.4, .8 + Math.random() * 0.4);
      return new THREE.ShaderMaterial({
        uniforms: u,
        vertexShader: VERTEX_SHADER,
        fragmentShader: FRAGMENT_SHADER,
        transparent: true,
      });
    }

    function disposable(material) {
      that.disposable.push(material);
      return material;
    }

    if (this.type == 'copper')
      return disposable(new THREE.MeshStandardMaterial({
        color: new THREE.Color(0xbf6c4d),
        normalMap: cloneTexture(TEXTURES.norm3, 1 / 1.4),
        transparent: true,
      }));

    if (this.type == 'iron')
      return disposable(new THREE.MeshPhongMaterial({
        color: new THREE.Color(0x777777),
        normalMap: cloneTexture(TEXTURES.norm3, 1 / 1.4),
        transparent: true,
      }));

    if (this.type == 'lead')
      return disposable(new THREE.MeshPhongMaterial({
        color: new THREE.Color(0x2f2f2f),
        normalMap: cloneTexture(TEXTURES.norm2, 1 / 1.1),
        normalScale: new THREE.Vector2(.2, .2),
        transparent: true,
      }));

    if (this.type == 'mercury')
      return disposable(new THREE.MeshPhongMaterial({
        color: new THREE.Color(0x707070),
        shininess: 2,
        normalMap: cloneTexture(TEXTURES.norm2, 1 / 1.1),
        normalScale: new THREE.Vector2(.8, .8),
        specular: new THREE.Color(0xffffff),
        transparent: true,
      }));

    if (this.type == 'sulfur')
      return disposable(new THREE.MeshPhongMaterial({
        color: new THREE.Color(0xf9e253),
        normalMap: TEXTURES.powder,
        normalScale: new THREE.Vector2(1.6, 1.6),
        transparent: true,
      }));

    if (this.type == 'gold')
      return disposable(metalMaterial(TEXTURES.gold, .06));

    if (this.type == 'catalyst')
      return disposable(metalMaterial(TEXTURES.catalyst, .06));
  }

  clone() {
    let tile = new Tile(this.type);
    if (this.board)
      tile.addToBoard(this.board, this.position);
    return tile;
  }

  dispose() {
    this.disposable.forEach(d => d.dispose());
    this.disposable.length = 0;
  }

  destroy() {
    console.log("destroyed");
    if (this.board)
      this.board.group.remove(this.group);
    this.dispose();
  }
}

class Sample {
  constructor(type1, type2) {
    this.rotation = 0;
    this.board = null;
    this.position = null;
    this.group = new THREE.Group();
    this.t1 = new Tile(type1);
    this.t2 = new Tile(type2);
  }

  rotate() {
    this.rotation = (this.rotation + 1) % 4;
    this._setPosition();
  }

  fits(pos) {
    return (0 <= pos.c && pos.c < BOARD_SIZE && 0 <= pos.r && pos.r < BOARD_SIZE);
  }

  otherPosition(pos) {
    return {
      c: Math.round(pos.c + Math.cos(this.rotation * Math.PI / 2)),
      r: Math.round(pos.r - Math.sin(this.rotation * Math.PI / 2))
    };
  }

  otherFits(pos) {
    return this.fits(this.otherPosition(pos));
  }

  _setPosition() {
    if (this.board == null)
      return;
    this.t1.setPosition(this.board, this.position);
    this.t2.setPosition(this.board, this.otherPosition(this.position));
  }

  setPosition(board, pos) {
    this.board = board;
    this.t1.group.visible = this.t2.group.visible = this.board != null;
    if (this.board == null)
      return;
    this.position = pos;
    this._setPosition();
  }

  randomOffset() {
    this.t1.setMapOffet(new THREE.Vector2(Math.random(), Math.random()));
    this.t2.setMapOffet(new THREE.Vector2(Math.random(), Math.random()));
  }

  getTiles() {
    return [this.t1.clone(), this.t2.clone()];
  }

  addToScene() {
    this.t1.addToScene();
    this.t2.addToScene();
  }
}

class Board {
  constructor(player) {
    this.player = player;
    this.material = new THREE.MeshPhongMaterial({color: 0xefefef});
    this.material.receiveShadow = true;
    this.mesh = new THREE.Mesh(GEOMETRIES.board, this.material);
    this.mesh.translation = GEOMETRIES.board.center();
    this.mesh.castShadow = true;
    this.group = new THREE.Group();
    this.group.add(this.mesh);
    this.group.board = this;
  }

  addToScene() {
    scene.add(this.group);
  }
}

async function init(rootElement) {
  domMain = rootElement;
  initDom();

  if (SHOW_FPS) {
    stats = new Stats();
    stats.showPanel(0);
    domMain.appendChild(stats.dom);
  }

  scene = new THREE.Scene();

  initCamera();
  initLights();
  initRenderer();

  domMain.appendChild(renderer.domElement);

  let controls = new THREE.OrbitControls(camera, renderer.domElement);
  controls.minDistance = .1;
  controls.maxDistance = 500;
  controls.maxPolarAngle = Math.PI / 2;
  controls.enablePan = true;
  controls.target.copy(scene.position);
  controls.update();

  await initAssets();

  objEach(TEXTURES, (key, t) => {
    t.wrapS = t.wrapT = THREE.RepeatWrapping;
  });
  TEXTURES.powder.repeat.set(1 / 4, 1 / 4);

  initMesh(); // create meshes (decoration & stuff)
  start(); // start the viewer logic
  render(); // start rendering via requestAnimationFrame()
}

function initDom() {
  $players = $('#players');
  PLAYERS.forEach(idx => {
    let $player = $('<div>').attr('id', 'player-' + idx).addClass('player');
    let $name = $('<div>').addClass('name');
    let $gold = $('<div>').addClass('gold').append($('<i>').addClass('icon')).append($('<span>').addClass('value'));
    let $catalyst = $('<div>').addClass('catalyst').append($('<i>').addClass('icon')).append($('<span>').addClass('value'));
    $player.append($name).append($gold).append($catalyst);
    $players.hide().append($player);
  });
}

function initCamera() {
  const $dom = $(domMain);
  camera = new THREE.PerspectiveCamera(70, $dom.width() / $dom.height(), 1, 10000);
  camera.position.set(0, 0, 5);
}

function initRenderer() {
  const $dom = $(domMain);
  renderer = new THREE.WebGLRenderer({antialias: false});
  renderer.setPixelRatio(window.devicePixelRatio);
  renderer.setSize($dom.width(), $dom.height());
  // renderer.autoClear = false;
  renderer.shadowMap.enabled = true;
  // let renderScene = new THREE.RenderPass(scene, camera);
  // bloomPass = new THREE.BloomPass(0.7);
  // let copyShader = new THREE.ShaderPass(THREE.CopyShader);
  // copyShader.renderToScreen = true;
  // composer = new THREE.EffectComposer(renderer);
  // composer.setSize(domMain.clientWidth, domMain.clientHeight);
  // composer.addPass(renderScene);
  // composer.addPass(bloomPass);
  // composer.addPass(copyShader);
  // composer.reset();
}

function initLights() {
  lights.ambient = new THREE.AmbientLight(0xffffff, .05);
  scene.add(lights.ambient);

  function createSpot(color) {
    let light = new THREE.SpotLight(color || 0xffffff, 1);
    light.castShadow = true;
    light.angle = 0.55;
    light.penumbra = 0.1;
    light.decay = .6;
    light.distance = 15;
    light.shadow.mapSize.width = 1024;
    light.shadow.mapSize.height = 1024;
    return light;
  }

  // board spots
  lights.boards = [];
  [0, 1].forEach(idx => {
    let spot = createSpot();
    spot.position.set(boardSign(idx) * 4.5, 10, 0);
    lights.boards.push(spot);
    scene.add(spot);
  });
}

async function initAssets() {
  function makePromise(container, loader, key) {
    let path = container[key];
    return new Promise(function (resolve, reject) {
      loader.load(path,
        // success
        function (data) {
          console.debug('loaded', path);
          container[key] = data;
          resolve(path);
        },
        // progress (who cares?)
        undefined,
        // failure
        function () {
          reject(new Error('could not load ' + path));
        })
    });
  }

  function buildPromises(container, loader) {
    return Object.keys(container)
      .filter(key => typeof container[key] == 'string')
      .map(key => makePromise(container, loader, key));
  }

  let geometryLoader = new THREE.BufferGeometryLoader();
  let textureLoader = new THREE.TextureLoader();
  textureLoader.path = TEXTURE_DIR;

  let promises = [];
  promises.push(...buildPromises(GEOMETRIES, geometryLoader));
  promises.push(...buildPromises(TEXTURES, textureLoader));
  return Promise.all(promises);
}

function initMesh() {
  // boards
  boards.push(...[new Board("A"), new Board("B")]);
  boards.forEach((board, idx) => {
    lights.boards[idx].target = board.group;
    board.addToScene();
  });

  if (INTERACTIVE) {
    cursorSample = new Sample('copper', 'lead');
    cursorSample.addToScene();
  }

  function createFullMesh(baseName, scale) {
    let mesh = new THREE.Mesh(GEOMETRIES[baseName], new THREE.MeshPhongMaterial({
      map: TEXTURES[baseName],
      specularMap: TEXTURES[baseName + 'Specular'],
      normalMap: TEXTURES[baseName + 'Normal'],
    }));
    mesh.scale.multiplyScalar(scale);
    mesh.translation = GEOMETRIES[baseName].center();
    return mesh;
  }

  // table
  let tableGroup = new THREE.Group();

  let tableWood = TEXTURES.wood2.clone();
  tableWood.repeat.set(2, 2);
  tableWood.offset.set(.35, .11);
  tableWood.needsUpdate = true;
  let table = new THREE.Mesh(new THREE.BoxGeometry(35, 1, 30), new THREE.MeshPhongMaterial({
    map: tableWood,
    normalMap: TEXTURES.wood2Normal,
    bumpMap: TEXTURES.wood2Bump,
    normalScale: new THREE.Vector2(.4, .4),
    shininess: 0,
  }));
  table.receiveShadow = true;
  table.position.set(0, -1, -3);
  tableGroup.add(table);

  // stand
  let standGroup = new THREE.Group();
  let stand = new THREE.Mesh(GEOMETRIES.stand, new THREE.MeshPhongMaterial({
    map: TEXTURES.wood,
  }));
  stand.translation = GEOMETRIES.stand.center();
  stand.receiveShadow = stand.castShadow = true;
  standGroup.add(stand);
  // tubes
  let tube = new THREE.Mesh(GEOMETRIES.tube, new THREE.MeshPhongMaterial({
    color: new THREE.Color(0xffffff),
    transparent: true,
    opacity: .8,
  }));
  tube.translation = GEOMETRIES.tube.center();
  tube.position.y = .85;
  tube.receiveShadow = tube.castShadow = true;
  standGroup.add(tube);
  let tube2 = tube.clone();
  tube2.position.x += .9 * 2;
  standGroup.add(tube2);

  standGroup.scale.multiplyScalar(.8);
  standGroup.rotation.y = 2 * Math.PI / 20;
  standGroup.position.set(-7.8, 0, -6);
  tableGroup.add(standGroup);

  // books
  let book = createFullMesh('book', .18);
  book.material.map = TEXTURES.bookLogo;
  book.receiveShadow = book.castShadow = true;
  books.b1 = book.clone();
  books.b1.rotation.set(Math.PI / 2, 0, -Math.PI / 2);
  books.b2 = books.b1.clone();
  books.b3 = books.b1.clone();
  books.b1.rotation.y = -Math.PI / 9;
  books.b1.position.set(.5, 1.4, -6.3);
  tableGroup.add(books.b1);
  books.b2.rotation.z = -.2;
  books.b2.position.set(-6.5, 1, -8.4);
  tableGroup.add(books.b2);
  books.b3.position.set(1.9, 1, -6.5);
  tableGroup.add(books.b3);
  books.main = book.clone();
  books.main.scale.multiplyScalar(1.2);
  books.main.rotation.set(0, -Math.PI / 9, 0);
  books.main.position.set(9, 0, -8);
  tableGroup.add(books.main);

  // candle
  let candle = createFullMesh('candle', .3);
  candle.position.set(0, .9, 0);
  candle.rotation.y = Math.random() * Math.PI;
  lights.candle = new THREE.PointLight(0xffffff, 1);
  // FIXME: performance
  // lights.candle.castShadow = true;
  lights.candle.position.set(0, 2.2, 0);
  lights.candle.distance = 50;
  lights.candle.decay = 5;
  let candleGroup = new THREE.Group();
  candleGroup.add(candle);
  candleGroup.add(lights.candle);
  candleGroup.position.set(-3, 0, -7);
  tableGroup.add(candleGroup);
  let c2 = candleGroup.clone();
  c2.position.set(4, 0, -7.3);
  tableGroup.add(c2);
  scene.add(tableGroup);
}

function render() {
  requestAnimationFrame(render);

  let delta = clock.getDelta();
  tick += delta;
  if (tick < 0) tick = 0;

  if (INTERACTIVE && turnIndex !== undefined) {
    raycaster.setFromCamera(mouse, camera);
    let intersect = raycaster.intersectObject(boards[turnIndex % 2].group, true);
    cursorSample.setPosition(null);
    if (intersect.length) {
      intersect = intersect[0];
      let board = intersect.object.parent.board;
      // get position in board coordinates
      let p = board.group.position.clone();
      p.sub(intersect.point);
      let pos = {
        x: Math.floor(-p.getComponent(0) / TILE_OFF) + H_BOARD_SIZE,
        z: Math.floor(-p.getComponent(2) / TILE_OFF) + H_BOARD_SIZE,
      };
      // if position is on board
      if (cursorSample.fits(pos)) {
        // position the sample
        cursorSample.setPosition(board, pos);
        // rotate to fit, if needed
        while (!cursorSample.otherFits(pos)) cursorSample.rotate();
        // update coord text
        // domCoord.textContent = "(" + pos.x + ", " + pos.z + ")";
      }
    }
  }

  boardTiles.forEach(b => b.forEach(tile => {
    if (!tile)
      return;
    if (tile.mesh.material.isShaderMaterial)
      tile.mesh.material.uniforms.time.value += 3 * delta;
    tile.group.position.y = TILE_Y;
    if (tile.morphing)
      tile.group.position.y += (Math.random() - .5) / 8;
    else
      tile.group.position.y = TILE_Y - .07 + Math.sin(tick * (.5 + tile.random) + tile.random * Math.PI * 2) / 26;
  }));

  if (SHOW_FPS) stats.begin();
  // TWEEN.update(tick * 1000);
  TWEEN.update();
  // renderer.clear();
  // composer.render(delta);
  renderer.render(scene, camera);
  if (SHOW_FPS) stats.end();
}

/*
function initEvents() {
  document.addEventListener('keyup', function (event) {
    event.preventDefault();
    console.debug(event.which);
    if (event.which == 82) {
      // r
      if (!INTERACTIVE)
        return;
      cursorSample.rotate();
    } else if (event.which == 32) {
      // space
      if (animating)
        return;
      send('next');
    }
  });

  domMain.addEventListener('mousemove', function (event) {
    if (!INTERACTIVE)
      return;
    event.preventDefault();
    const $dom = $(domMain);
    let x = event.clientX - document.getElementById('container').offsetLeft;
    let y = event.clientY - document.getElementById('container').offsetTop;
    mouse.x = (x / $dom.width()) * 2 - 1;
    mouse.y = -(y / $dom.height()) * 2 + 1;
  });

  domMain.addEventListener('click', function (event) {
    if (!INTERACTIVE)
      return;
    event.preventDefault();
    if (cursorSample.board != null) {
      let [t1, t2] = cursorSample.getTiles();
      cursorSample.randomOffset();
      t1.addToScene();
      t2.addToScene();
      tiles.push(t1);
      tiles.push(t2);
      if (false) {
        new TWEEN.Tween(lights.boards[turnIndex % 2])
          .easing(TWEEN.Easing.Quadratic.InOut)
          .to({intensity: 1, angle: .5}, 300)
          .start();
        new TWEEN.Tween(lights.boards[(turnIndex + 1) % 2])
          .easing(TWEEN.Easing.Quadratic.InOut)
          .to({intensity: .4, angle: .48}, 300)
          .start();
      }
    }
  });
}
*/

function renderTurn(index) {
  console.debug("rendering turn", index);

  const ANIMATE_SAVE = ANIMATE;
  const previousTurnData = turnCache[index - 1], turnData = turnCache[index];
  console.assert(turnData.turn[0] === index);

  function setCatalyst(pIdx, value, relative) {
    if (relative)
      catalysts[pIdx] += value;
    else
      catalysts[pIdx] = value;
    $('#player-' + pIdx).find('.catalyst .value').text(value);
  }

  objEach(turnData.players, (pid, p, pIdx) => {
    const dom = $('#player-' + pIdx);
    dom.find('.name').text(p.name);
    setCatalyst(pIdx, 0);
    if (p.sample)
      p.sample.forEach((material, idx) => samples[pIdx][idx].setType(API_MATERIAL[material]));
  });

  if (false/* ANIMATE LIGHTS */) {
    if (ANIMATE) {
      new TWEEN.Tween(lights.boards[(turnIndex + 1) % 2])
        .easing(TWEEN.Easing.Quadratic.InOut)
        .to({intensity: 1}, 200)
        .start();
      new TWEEN.Tween(lights.boards[(turnIndex) % 2])
        .easing(TWEEN.Easing.Quadratic.InOut)
        .to({intensity: .4}, 200)
        .start();
    } else {
      lights.boards[(turnIndex + 1) % 2].intensity = 1;
      lights.boards[(turnIndex) % 2].intensity = .4;
    }
  }

  function updateBoards() {
    ANIMATE = false;
    objEach(turnData.players, (pid, p, pIdx) => {
      const dom = $('#player-' + pIdx);
      // gold
      dom.find('.gold .value').text(p.score);
      // bench
      p.bench.forEach((apiMaterial, idx) => {
        const material = API_MATERIAL[apiMaterial];
        if (boardTiles[pIdx][idx]) {
          if (!material) {
            boardTiles[pIdx][idx].destroy();
            boardTiles[pIdx][idx] = null;
          } else {
            boardTiles[pIdx][idx].setType(material);
          }
        } else if (material) {
          const tile = new Tile(material);
          tile.addToBoard(boards[pIdx], indexToPosition(idx));
          boardTiles[pIdx][idx] = tile;
        }
      });
    });
    ANIMATE = ANIMATE_SAVE;
  }

  if (!previousTurnData || !ANIMATE) {
    // we don't have the previous state or don't want to animate; just update everything instantly
    updateBoards();
    turnEnded();
    // exit early
    return;
  }

  let player, playerIdx, pidToIdx = {};
  animating = true;

  // find the player who played this turn
  objEach(turnData.players, (pid, p, pIdx) => {
    const dom = $('#player-' + pIdx);
    new TWEEN.Tween({v: parseInt(dom.find('.gold .value').text())})
      .easing(TWEEN.Easing.Quadratic.Out)
      .to({v: p.score}, 300)
      .onUpdate(function () {
        dom.find('.gold .value').text(parseInt(this.v));
      })
      .start();
    pidToIdx[pid] = pIdx;
    if (p.just_played) {
      playerIdx = pIdx;
      player = p;
    }
  });
  console.assert(player != undefined);
  console.assert(playerIdx != undefined);

  const board = boardTiles[playerIdx];

  if (!player.history.length) {
    const d = .7;
    let p = boards[playerIdx].group.position, x = p.x;
    new TWEEN.Tween(p)
      .easing(TWEEN.Easing.Quadratic.Out)
      .to({x: [x - d, x + d, x - d, x]}, 400)
      .start();
  }

  // unroll history
  let actions = [];

  for (const hist of player.history) {
    if (hist.type == 'ACTION_PLACER') {
      actions.push(() => {
        [hist.pos1, hist.pos2].forEach((pos, posIdx) => {
          const idx = positionToIndex(pos),
            material = API_MATERIAL[player.sample[posIdx]];
          if (board[idx]) {
            console.assert(false, "tried to add", material, "to", pos, "but there is", board[idx]);
            board[idx].destroy();
            board[idx] = null;
          }
          let tile = board[idx] = new Tile(material);
          tile.addToBoard(boards[playerIdx], pos);
          tile.group.position.y = 12;
          new TWEEN.Tween(tile.group.position)
            .easing(TWEEN.Easing.Quadratic.Out)
            .to({y: TILE_Y}, 300)
            .onStart(() => tile.morphing = true)
            .onComplete(() => tile.morphing = false)
            .start();
        });
        return 300;
      });
    } else if (hist.type == 'ACTION_TRANSMUTER') {
      actions.push(() => {
        const origPos = hist.pos;
        const origIdx = positionToIndex(origPos);
        const origTile = board[origIdx];

        if (!origTile) {
          // catastrophic, but I have no idea how it can happen
          console.assert(false, "no origTile", origPos, origIdx);
          return;
        }

        // dfs to propagate to other tiles
        let transmuted = [], maxDepth = 0, area = 0;
        let transmute = (pos, depth) => {
          const idx = positionToIndex(pos);
          // already visited
          if (transmuted.indexOf(idx) != -1)
            return;
          // out of bounds
          if (!(0 <= idx && idx < BOARD_SIZE * BOARD_SIZE))
            return;
          const tile = board[idx];
          if (!tile)
            return;
          // not the same type
          if (tile.type != origTile.type)
            return;
          transmuted.push(idx);
          maxDepth = Math.max(maxDepth, depth);
          tile.morphTo(tile.transmutesTo(), depth * 30);
          board[idx] = null;
          area++;
          for (const dir of DIRECTIONS)
            transmute({r: pos.r + dir.r, c: pos.c + dir.c}, depth + 1);
        };
        transmute(origPos, 0);

        if (origTile.transmutesTo() == 'catalyst') {
          setCatalyst(playerIdx, parseInt(area / 2), true);
        }
        return (maxDepth + 1) * 35 + 300;
      });

    } else if (hist.type == 'ACTION_CATALYSER') {
      actions.push(() => {
        console.warn("SUCH CATALYSER");
        const targetPlayerIdx = pidToIdx[hist.apprentice_id],
          targetMaterial = API_MATERIAL[hist.new_type],
          pos = hist.pos;
        console.assert(targetMaterial != 'sulfur' && targetMaterial != 'gold');
        boardTiles[targetPlayerIdx][positionToIndex(pos)].morphTo(targetMaterial);
        setCatalyst(playerIdx, -1, true);
        return 300;
      });
    }
  }

  function checkTurnEnd() {
    animating = false;
    const computedBench = board.map(tile => tile ? tile.type : null),
      serverBench = player.bench.map(material => API_MATERIAL[material]);
    let i = 0, errors = [];
    while (i < computedBench.length && i < serverBench.length) {
      if (computedBench[i] != serverBench[i])
        errors.push(i);
      i++;
    }
    if (errors.length) {
      console.assert(false, errors, computedBench, serverBench);
      updateBoards();
    }
    turnEnded();
  }

  function animate() {
    if (!actions.length) {
      checkTurnEnd();
      return;
    }
    let func = actions.shift();
    let delay = func();
    setTimeout(animate, delay);
  }

  animate();
}

function initSamples(initialSamples) {
  boards.forEach((board, idx) => {
    samples[idx] = [];
    initialSamples.forEach((material, tIdx) => {
      const tile = new Tile(API_MATERIAL[material]);
      tile.group.scale.multiplyScalar(.8);
      tile.group.position.x = -boardSign(idx) * TILE_OFF * (H_BOARD_SIZE + .5);
      tile.group.position.z = TILE_OFF * tIdx + boardSign(idx) * 2 * TILE_OFF - H_TILE_OFF;
      board.group.add(tile.group);
      samples[idx].push(tile);
    });
  });
}

function start() {
  boardTiles = boards.map(e => []);

  camera.position.set(0, 10, 3.5);
  camera.lookAt(scene.position);

  boards.forEach((board, idx) => {
    let g = board.group, sign = boardSign(idx);
    g.position.x = sign * 16;
  });

  books.main.rotation.set(0, 0, 0);
  books.main.position.set(0, 0, 0);

  let bookPos = new TWEEN.Tween(books.main.position)
    .easing(TWEEN.Easing.Quadratic.InOut)
    .to({x: 9, z: -8}, 1000);
  let bookRot = new TWEEN.Tween(books.main.rotation)
    .easing(TWEEN.Easing.Quadratic.InOut)
    .to({y: -Math.PI / 9}, 1000);

  bookPos.start();
  bookRot.start().onComplete(() => {
    boards.forEach((board, idx) => {
      let g = board.group, sign = boardSign(idx);
      new TWEEN.Tween(g.position)
        .easing(TWEEN.Easing.Quadratic.Out)
        .to({x: sign * 4.5}, 800)
        .start()
        .onComplete(() => {
          $players.fadeIn(1000);
          let e = new THREE.Object3D();
          e.position.copy(board.group.position);
          scene.add(e);
          lights.boards[idx].target = e;
        });
    });
  });
}

function animateWinner() {
  let data = turnCache[lastTurn - 1];
  if (!data)
    return;
  let pids = Object.keys(data.players), p0 = data.players[pids[0]].score, p1 = data.players[pids[1]].score;
  let winners = [], losers = [];
  if (p0 == p1) {
    winners = [0, 1];
  } else if (p0 < p1) {
    winners.push(1);
    losers.push(0);
  } else {
    winners.push(0);
    losers.push(1);
  }
  winners.forEach(idx => {
    $('#player-' + idx).addClass('winner');
    setTimeout(() => lights.boards[idx].intensity = 1, 300);
    new TWEEN.Tween(boards[idx].group.position)
      .easing(TWEEN.Easing.Sinusoidal.InOut)
      .repeat(Infinity)
      .to({y: [1, 0]}, 3000)
      .start();
  });
  losers.forEach(idx => {
    setTimeout(() => lights.boards[idx].intensity = .4, 300);
    boards[idx].group.scale.set(.8, .8, .8);
  });
}

function turnEnded() {
  console.debug("turn end");
  if(window.END_TURN_CALLBACK) window.END_TURN_CALLBACK();
}
