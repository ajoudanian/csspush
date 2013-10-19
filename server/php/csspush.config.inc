<?php
/**
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2013 aramalipoor
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of
 *  this software and associated documentation files (the "Software"), to deal in
 *  the Software without restriction, including without limitation the rights to
 *  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 *  the Software, and to permit persons to whom the Software is furnished to do so,
 *  subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 *  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 *  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 *  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */   

$config = array(
  /**
   * Configure which paths are allowed to push
   * css changes into and which are restricted
   * using an array of regular expressions.
   */
  'paths' =>
    array(
      /**
       * This array contains restricted paths that client not change.
       *
       * A few examples are:
       *    array('.*jquery\..*')     - excludes any css from jquery libraries
       *                                  (i.e. filename contains 'jquery' with a preceding dot)
       *    array('.*\.min\.css')     - excludes any css file which has a .min.css in its name
       */
      'deny' => array(),
      
      /**
       * This array contains allowed paths as regular expressions.
       *
       * A few examples are:
       *    array('.*')                     - allows every css file at any location to be changed
       *    array('.*css\/skins/.*')        - allows those css files that are inside any css/skins/ directory
       *    array('assets\/.*', 'css\/.*')  - allows only assets/ or css/ directory at site's root (and not other assets/ directories)
       */
      'allow' => array()
      
      /**
       * NOTE
       * Deny-rules will be applied first and allow-rules the second.
       * You can deny every directory and then allow only a single directory like this:
       *    'deny'  => array('.*'),
       *    'allow' => array('\/css\/')
       */
    ),
  'users' =>
    array(
      /**
       * You can define various users for csspush server
       * each user has a secret key and allowed paths (in regular expressions)
       */
      'admin' => array(
        'key' => '12345'
        
        // NOTE that below configurations will override global 'path' configuration we defined earlier above
        'deny' => array(),
        'allow' => array()
      )
    )
);