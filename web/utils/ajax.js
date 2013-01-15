
function xmlhttpPostConcat(strURL, query, resultTag) {
  xmlhttpPostHandle(strURL, query, resultTag, 1);
}

function xmlhttpPost(strURL, query, resultTag) {
  xmlhttpPostHandle(strURL, query, resultTag, 0);
}

function xmlhttpPostHandle(strURL, query, resultTag, concat) {
    var xmlHttpReq = false;

    // Mozilla/Safari
    if (window.XMLHttpRequest) {
        xmlHttpReq = new XMLHttpRequest();
    }
    // IE
    else if (window.ActiveXObject) {
        xmlHttpReq = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlHttpReq.open('POST', strURL, true);
    xmlHttpReq.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
    xmlHttpReq.onreadystatechange = function() {
        if (xmlHttpReq.readyState == 4) {
            if (concat == 1) {
               document.getElementById(resultTag).innerHTML += xmlHttpReq.responseText;
            } else {
               document.getElementById(resultTag).innerHTML = xmlHttpReq.responseText; 
            }
        }
    }
    xmlHttpReq.send(query);
}

