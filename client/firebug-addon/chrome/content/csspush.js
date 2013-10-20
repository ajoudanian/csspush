FBL.ns(function () {
  var options = {
    username: branch.getCharPref('username'),
    key: branch.getCharPref('key'),
    prependDomain: branch.getBoolPref('prependDomain'),
    remoteEndpoint: branch.getCharPref('remoteEndpoint')
  };
  
  branch.addObserver("", {
    observe: function(subject, topic, data)
    {
      if (topic != "nsPref:changed") {
        return;
      }
      
      options[data] = branch.getCharPref(data);
    }
  }, false);
  
  with (FBL) {
    var ajax = {
      prepareRequest: function(callback) {
        var request = new XMLHttpRequest();
        request.onreadystatechange = function() {
          if (request.readyState == 4) {
            if(request.status == 200) {
              var json = null;
              
              try {
                json = eval("(" + request.responseText + ")");
                
                callback({
                  json: json,
                  text: request.responseText
                });
              } catch(e) {
                alert('CSSPush Error: Ajax error, ' + e);
              }
            } else {
              alert('CSSPush Error: Http request error with code ' + request.status);
            }
          }
        }
        
        return request;
      },
      
      arrayToQueryString: function(array, pre, post) {
        if(!pre) pre = '';
        if(!post) post = '';
        
        var paramsArray = [];
        
        for(var key in array) {
          if(typeof array[key] === 'object') {
            paramsArray.push(this.arrayToQueryString(array[key], pre + key + post + '[', ']'));
          } else {
            paramsArray.push(pre + key + post + '=' + encodeURIComponent(array[key]));
          }
        }
        
        return paramsArray.join('&');
      },
      
      post: function(url, params, callback) {
        if(typeof params === 'function') {
          callback = params;
          params = {};
        }
        
        var xhr = this.prepareRequest(callback);
        xhr.open("POST", url, true);
        xhr.setRequestHeader("Content-type","application/x-www-form-urlencoded");
        xhr.send(this.arrayToQueryString(params));
      },
      
      get: function(url, params, callback) {
        if(typeof params === 'function') {
          callback = params;
          params = {};
        }
        
        var xhr = this.prepareRequest(callback);
        url = url + (url.indexOf('?') > -1 ? '&' : '?');
        xhr.open("GET", url + this.arrayToQueryString(params), true);
      },
      
      rpc: function(params, callback) {
        if(typeof params === 'function') {
          callback = params;
          params = {};
        }
        
        var endpoint = options.remoteEndpoint,
            prepdomain = options.prependDomain,
            base;
        
        params.username = options.username;
        params.key = options.key;
        params.baseUrl = '';
        params.url = Firebug.currentContext.window.document.location.href;
        
        if(Firebug.currentContext.window.document) {
          base = Firebug.currentContext.window.document.getElementsByTagName('base');
          if(base.length > 0) {
            params.baseUrl = base[0].href;
          }
        }
        
        if(prepdomain) {
          endpoint = ('https:' == document.location.protocol ? 'https://' : 'http://')
            + Firebug.currentContext.window.document.domain + endpoint;
        }
        
        // Commit changes to server
        ajax.post(endpoint, params, function(resp) {
          
          if(!resp.json.success) {
            alert('CSSPush Remote Error: ' + resp.json.error);
          }
          
          callback(resp.json);
        });
      }
    };
  
    var CssListener = {
      changedStylesheets: [],
      
      // CSS Listeners
      onCSSInsertRule: function(styleSheet, cssText, ruleIndex)
      {
        this.stylesheetSetDirty(styleSheet, styleSheet.cssRules[ruleIndex]);
        this.selection = null;
      },

      onCSSDeleteRule: function(styleSheet, ruleIndex)
      {
        this.stylesheetSetDirty(styleSheet, styleSheet.cssRules[ruleIndex]);
        this.selection = null;
      },

      onCSSSetProperty: function(style, propName, propValue, propPriority, prevValue, prevPriority, rule, baseText)
      {
        this.stylesheetSetDirty(rule.parentStyleSheet, rule);
        this.selection = null;
      },

      onCSSRemoveProperty: function(style, propName, prevValue, prevPriority, rule, baseText)
      {
        this.stylesheetSetDirty(rule.parentStyleSheet, rule);
        this.selection = null;
      },
      
      // Internals
      stylesheetSetDirty: function(stylesheet, changedRule) {
        
        if(!stylesheet || !stylesheet.href || !changedRule || !changedRule.selectorText) {
          return false;
        }
        
        for(var i = 0; i < this.changedStylesheets.length; i++) {
          if(this.changedStylesheets[i].href === stylesheet.href) {
            return false;
          }
        }
      
        this.changedStylesheets.push(stylesheet);
        
        return true;
      },
      
      serializeChanges: function() {
        var ccr = this.changedStylesheets,
            changes = [];

        for(var i = 0; i < ccr.length; i++) {
          changes.push({
            url: ccr[i].href,
            content: this.getStylesheetText(ccr[i])
          });
        }
        
        this.resetChanges();
        return {changes: changes};
      },
      
      resetChanges: function() {
        this.changedStylesheets = [];
        this.forceChange = false;
      },
      
      getStylesheetText: function(sheet) {
        var rules = sheet.cssRules, text = '';
        
        if(rules) {
          for(var j = 0; j < rules.length; j++) {
            text += rules[j].cssText + "\n";
          }
        }
        
        return text;
      }
    };
  
    Firebug.CSSPushModule = extend(Firebug.Module,
    {
      onSaveCommit: function(context) {
        var sheets = '';
        
        if(CssListener.changedStylesheets.length == 0) {
          alert('There are no css changes to be saved.');
          return;
        }
        
        for(var i in CssListener.changedStylesheets) {
          sheets += CssListener.changedStylesheets[i].href+"\n";
        }
        
        ajax.rpc(CssListener.serializeChanges(true), function(json) {
          if(json.success) {
            alert(
              "CSS Push Commited.\n\n"+
                json.result.join("\n")+
                "\n\n"+
                json.error.join("\n")
            );
          } else {
            alert("CSS Push Save Error: " + json.error);
          }
        });
      },
      
      resetChanges: function(context) {
        CssListener.resetChanges();
      }
    });

    Firebug.registerModule(Firebug.CSSPushModule);
    Firebug.CSSModule.addListener(CssListener);
  }
});

