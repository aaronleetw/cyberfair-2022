function CtrlNavTransparancy() {
    var scrollPos = $(document).scrollTop();
    var targetOpacity = scrollPos / 100;
    var stOpacity = scrollPos / 400;
    $('.navbar').attr('style', 'background-color: rgba(52, 58, 64, ' + targetOpacity + ') !important;');
    if (stOpacity >= 1) {
        $('#scrollTopBtn').attr('style', 'opacity: 1');
    } else {
        $('#scrollTopBtn').attr('style', 'opacity: 0');
    }
};

$(function () {
    // when document fully loaded
    $(document).ready(function () {
        CtrlNavTransparancy();
    });
    $(window).scroll(function () {
        CtrlNavTransparancy();
    });
});

$("#scrollTopBtn").click(function () {
    $("html, body").stop().animate({
        scrollTop: 0
    }, 300);
});