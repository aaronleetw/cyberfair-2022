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

function moveBackButtons() {
    if ($("nav div.buttons").css("position") == "fixed") {
        $("nav div.buttons").css("position", "relative");
        $("nav div.buttons").css("top", "0");;
        $("nav div.buttons").css("right", "0");
    }
    else {
        $("nav div.buttons").css("position", "fixed");
        $("nav div.buttons").css("top", "10px");
        $("nav div.buttons").css("right", "10px");
    }
}
$(document).ready(function () {
    if (!localStorage.noFirstVisit) {
        $("body").append(`
        <div class="modal fade" id="firstVisit" data-bs-backdrop="static" data-bs-keyboard="false" tabindex="-1" aria-labelledby="staticBackdropLabel" aria-hidden="true">
            <div class="modal-dialog modal-lg">
                <div class="modal-content">
                <div class="modal-header">
                    <h5 class="modal-title text-center" id="staticBackdropLabel">Welcome 歡迎!</h5>
                </div>
                <div class="modal-body text-center">
                    Greetings from Taiwan! This website is made by <a href="#" onclick="clickedModal(2)">a group of students</a> from Taipei Fuhsing Private School in Taiwan.
                    It is optimized for 1920x1080 screens, and might be unusable with other screens.<br><br>
                    你好! 本網站由台北市私立復興實驗中學的<a href="#" onclick="clickedModal(3)">一群學生</a>製作。這個網站針對 1920x1080 螢幕最佳化，其他螢幕可能會無法使用。
                    <br><br><span class="small text-muted">This message will only show once. 這個訊息只會顯示一次。</span>
                </div>
                <div class="modal-footer text-center">
                    <button type="button" class="btn btn-primary" data-bs-dismiss="modal" onclick="clickedModal(0)">English Version</button>
                    <button type="button" class="btn btn-primary" data-bs-dismiss="modal" onclick="clickedModal(1)">繁體中文版</button>
                </div>
                </div>
            </div>
        </div>
        `);
        $("#firstVisit").modal('show');
    }
});

function clickedModal(lang) {
    localStorage.noFirstVisit = true;
    currPath = window.location.pathname.substring(0, 5) == "index" || window.location.pathname == '/' ? '/' : window.location.pathname.substring(3);
    currLang = window.location.pathname.substring(0, 5) == "index" || window.location.pathname == '/' ? 'en' : window.location.pathname.substring(1, 3);
    if (lang == 0) {
        if (currPath == '/') { return; }
        if (currLang == 'en') { return; }
        location.href = "/en" + currPath;
    } else if (lang == 1) {
        if (currLang == 'zh') { return; }
        location.href = "/zh" + currPath;
    } else if (lang == 2) {
        location.href = "/en/conclusion/team.html";
    } else if (lang == 3) {
        location.href = "/zh/conclusion/team.html";
    }
}

function switchLang(lang) {
    currPath = window.location.pathname.substring(0, 5) == "index" || window.location.pathname == '/' ? '/' : window.location.pathname.substring(3);
    if (lang == 'en' && currPath == '/') { location.href = '/'; return; }
    location.href = "/" + lang + currPath;
}