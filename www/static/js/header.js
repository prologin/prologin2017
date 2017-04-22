let xx = 420, yy = 442;

$(function () {
  const $brand = $('.navbar-brand');

  function rotateInterpolation(from, to, x, y, extra) {
    from = [(extra || "") + " rotate(", from, ",", x, ",", y, ")"].join("");
    to = [(extra || "") + " rotate(", to, ",", x, ",", y, ")"].join("");
    return d3.interpolateString(from, to);
  }

  $.when(
    $.ajax({url: '/static/logo.outline.svg', dataType: 'text'}),
    $.ajax({url: '/static/logo.text.svg', dataType: 'text'})
  ).then((outline, text) => {
    const $outline = $(outline[0]), $text = $(text[0]);
    $brand.append($outline).append($text);
    $outline.css({
      width: 112,
    });
    $text.css({
      width: 210,
      top: 20,
      left: 150,
    });

    const svgOutline = d3.select('.navbar-brand > svg:first-child'),
      eDragons = svgOutline.selectAll("#dragtext"),
      eStar = svgOutline.selectAll('#star'),
      eSymbols= svgOutline.selectAll('#symbols');

    const fromDragons = Math.random() * 360, fromStar = Math.random() * 360;

    function animateDragons() {
      eDragons
        .transition()
        .duration(19000)
        .ease("linear")
        .attrTween("transform", (d, i, a) => rotateInterpolation(fromDragons, fromDragons - 359, 426, 511))
        .each("end", animateDragons);
    }

    function animateStar() {
      eStar
        .transition()
        .duration(29000)
        .ease("linear")
        .attrTween("transform", (d, i, a) => rotateInterpolation(fromStar, fromStar + 359, 229 - 200, 83 - 200, "translate(400,626)"))
        .each("end", animateStar);
    }

    function animateSymbols() {
      eSymbols
        .transition()
        .duration(29000 / 3)
        .ease("linear")
        .attrTween("opacity", () => ((t) => Math.abs(Math.cos(t * 2 * Math.PI + .08))))
        .each("end", animateSymbols);
    }

    animateDragons();
    animateStar();
    animateSymbols();

  });
});
