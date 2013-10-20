const prefService = Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService);
const branch = prefService.getBranch("extensions.firebug.CSSPush.");