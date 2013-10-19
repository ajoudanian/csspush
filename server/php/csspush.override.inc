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

/**
 * In this file,
 * you can override default behaviour of CSSPush various functions
 * to meet your requirments and software architecture.
 */

/**
 * override_get_physical_path()
 *
 * If you use mode_rewrite and finding your css files real path is not
 * as usual, you should use this function to override default csspush behaviour (replacing domain name with document_root)
 *
 * @param string  $path       Physical path found by default function (replacing domain name with document_root)
 * @param array   $stylesheet Holds changed stylesheet information with structure like ('url' => string, 'content' => string)
 *
 * @return string Absolute css file physical path on server (e.g /home/user/public_html/css/main.css)
 */
function override_get_physical_path($path, $stylesheet) {
  // $stylesheet['content'] Holds css content (before manipulation)
  // $stylesheet['url']     Holds css web url
  return $path;
}

/**
 * override_manipulate_content()
 *
 * If you need custom manipulation on css content use this function
 *
 * @param string  $content    Manipulated css content using default csspush manipulate_content function.
 * @param array   $stylesheet Holds changed stylesheet information with structure like ('url' => string, 'content' => string)
 *
 * @return string manipulated css content to be written on css file
 */
function override_manipulate_content($content, $stylesheet) {
  // $stylesheet['content'] holds original css content
  // $stylesheet['url'] holds css web url
  // $stylesheet['path'] holds css file physical path
  return $content;
}