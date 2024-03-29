window.chatmax = {
    username: null,
    password: null,
    scramble: function(s) {
        var _1 = [];
        for (var i = 0; i < s.length; i++) {
            _1.push(s.charCodeAt(i).toString());
        }
        return _1.join("-");
    },
    validMsgStr: function(s) {
        if (s.length == 0) {
            return false;
        }
        if (s.charCodeAt(0) == 0) {
            return false;
        }
        return true;
    },
    fetchMessages: function() {
        var _2 = false;
        if (_2) {
            var _3 = {
                from: "jeeves",
                subject: "Your coat is ready!",
                date: (new Date()).toString(),
                body: "Sir, your coat is ready at the mansion!"
            };
            this.handleMessageResponse([_3]);
        } else {
            var _4 = "$" + Math.random().toString().replace(".", "") + "?pw=" + this.scramble(this.password) + "&un=" + this.scramble(this.username) + ".inbox";
            var _5 = new XMLHttpRequest();
            _5.addEventListener("load", function() {
                console.log("Raw inbox string: " + this.responseText);
                if (this.responseText.indexOf("<title>403 Forbidden</title>") != -1) {
                    alert("Error: login attempt failed :-(.");
                    return;
                }
                var _6 = this.responseText.split(/\/\/END_MSG\/\/\0*/);
                var _7 = [];
                for (var i = 0; i < _6.length; i++) {
                    if (chatmax.validMsgStr(_6[i])) {
                        var _8 = "(function(){ return " + _6[i] + ";})()";
                        _7.push(eval(_8));
                    }
                }
                console.log("Unmarshalled inbox: " + _7);
                Ext.get("welcomeDiv").update("Welcome " + chatmax.username + "!");
                chatmax.handleMessageResponse(_7);
            });
            _5.addEventListener("error", function() {
                alert("Error: Inbox fetch failed! Did you specify a valid username and password?");
            });
            _5.open("GET", _4);
            _5.send();
        }
    },
    handleMessageResponse: function(_9) {
        for (var i = 0; i < _9.length; i++) {
            var _a = _9[i];
            var _b = {
                tag: "textarea readonly",
                id: "msg" + i,
                style: "width:50%;height:100px;text-align:left;margin-left:0;resize:none;overflow:scroll",
                html: "From: " + _a.from + "\n" + "Subject: " + _a.subject + "\n" + "Date: " + _a.date + "\n" + _a.body
            };
            Ext.core.DomHelper.append("messagesDiv", _b);
        }
    }
};
Ext.require(["Ext.window.MessageBox", "Ext.tip.*"]);
Ext.onReady(function() {
    chatmax.password = null;
    var _c = function(_d, _e) {
        if (_d == "ok") {
            chatmax.password = _e;
            chatmax.fetchMessages();
        } else {
            setTimeout(function() {
                Ext.MessageBox.prompt("Login", "Password:", _c);
            }, 0);
        }
    };
    Ext.MessageBox.prompt("Login", "Password:", _c);
    chatmax.username = null;
    var _f = function(_10, _11) {
        if (_10 == "ok") {
            chatmax.username = _11;
            Ext.MessageBox.prompt("Login", "Password:", _c);
        } else {
            setTimeout(function() {
                Ext.MessageBox.prompt("Login", "Username:", _f);
            }, 0);
        }
    };
    Ext.MessageBox.prompt("Login", "Username:", _f);
    Ext.get("sendMessageButton").on("click", function() {
        var _12 = new Ext.form.Panel({
            title: "New Message",
            width: 500,
            height: 400,
            floating: true,
            modal: true,
            closable: true,
            defaultType: "textfield",
            items: [{
                fieldLabel: "To",
                name: "to"
            }, {
                fieldLabel: "Subject",
                name: "subject"
            }, {
                fieldLabel: "Body",
                xtype: "textareafield",
                name: "body"
            }],
            buttons: [{
                text: "Send",
                handler: function() {
                    var _13 = _12.items;
                    var _14 = "{\"from\": \"" + chatmax.username + "\"," + "\"subject\": \"" + _13.items[1].value + "\"," + "\"date\": \"" + (new Date()).toString() + "\"," + "\"body\": \"" + _13.items[2].value.replace("\n", "\\n", "g").replace("\r", "\\r", "g") + "\"}";
                    _14 += "//END_MSG//";
                    var _15 = 16 - (_14.length % 16);
                    if ((_15 > 0) && (_15 != 16)) {
                        while (_15-- > 0) {
                            _14 += String.fromCharCode(0);
                        }
                    }
                    console.log("Sending message to user: " + _13.items[0].value);
                    var url = "$" + Math.random().toString().replace(".", "") + "?pw=" + chatmax.scramble(chatmax.password) + "&un=" + chatmax.scramble(chatmax.username) + "&msg=" + chatmax.scramble(_14) + "&to=" + chatmax.scramble(_13.items[0].value) + ".inbox";
                    var xhr = new XMLHttpRequest();
                    xhr.addEventListener("load", function() {
                        if (this.responseText.indexOf("Msg Sent!") != -1) {
                            alert("Message successfully sent!");
                        } else {
                            alert("Received unexpected response: " + this.responseText);
                        }
                    });
                    xhr.addEventListener("error", function() {
                        alert("Error: Message send failed :-(.");
                    });
                    xhr.open("GET", url);
                    xhr.send();
                    _12.destroy();
                }
            }, {
                text: "Cancel",
                handler: function() {
                    _12.destroy();
                }
            }]
        });
        _12.show();
    });
});