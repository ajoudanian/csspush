csspush
=======

A simple toolkit for webpage designers to push CSS changes right from Firebug to their server.

Installation
=======
CSSPush consists of a server-side listener and a client on your browser. Below you can see a minimal and basic installation guide.

Server-side
-------
  1. Grab a copy of files inside `/server/php` directory.
  2. Modify `csspush.config.inc` settings to whatever you want.
  3. Upload these files directory to your server in a directory (e.g. `/csspush`)
     so you have web access to `example.com/csspush/csspush.server.php`

Client-side
-------
  1. Install latest version of Firefox browser.
  2. Install latest Firebug add-on.
  3. Install `csspush-firebug-addon.xpi` from `/client/firebug-addon` on your firefox.
  4. Goto your Firefox menu `Tools` » `Addon` » `CSSPush` » `Options` and configure your settings.

How to Use
=======
 Open a webpage on your server (e.g. `example.com/sample.php`) then open your Firebug (press `F12`).
 Now when you Inspect an element and look at CSS tab you can see a `Save` button (and a `Reset`).
 Easily change any CSS selector or property (which is external and inside a file) then click on `Save` button
 to commit your changes to your server.

