function loadImage(url) {
  return new Promise((resolve, reject) => {
    function loaded() {
      unbindEvents();
      resolve(image);
    }

    function errored() {
      unbindEvents();
      reject(image);
    }

    function unbindEvents() {
      image.onload = null;
      image.onerror = null;
      image.onabort = null;
    }

    const image = new Image();
    image.onload = loaded;
    image.onerror = errored;
    image.onabort = errored;
    image.src = url;
  });
}

$(function () {
  const $brand = $('.navbar-brand');

  Promise.all(['circle', 'star', 'symbols', 'text'].map(e => '/static/logo.' + e + '.png').map(loadImage))
    .then((images) => {
      const [circle, star, symbols, text] = images.map($);
      circle.addClass('anim-rotate').css({
        animationDuration: '19s'
      });
      star.addClass('anim-rotate').css({
        animationDuration: '21s',
        animationDirection: 'reverse',
        top: 41,
        left: 40,
      });
      symbols.addClass('anim-blink').css({
        animationDuration: '4.2s',
        top: 40,
        left: 54,
      });
      text.addClass('anim-appear').css({
        top: 30,
        left: 155,
      });
      $brand.append(circle).append(star).append(symbols).append(text);
    });
});
