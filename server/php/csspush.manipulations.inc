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
 * various manipulations on css files are defined.
 * Such as making css properties cross-browser,
 * or converting rgb() representation of colors to equivalent hex representation.
 *
 * Each manipulation consists of a regular expression pattern, and a manipulating function.
 * This pattern and anonymous function will be used in preg_replace_callback method.
 */

$manipulations = array(

  // cross-browser and vendor prefix generations
  '/(-[a-zA-Z]*-)?(border-radius|box-shadow|text-shadow)\s*:([^;]*);/im' => function($matches) {
    $value = clean_property_value($matches[3]);
    return trim('
      -ms-'.$matches[2].': '.$value.';
      -webkit-'.$matches[2].': '.$value.';
      -o-'.$matches[2].': '.$value.';
      -moz-'.$matches[2].': '.$value.';
      '.$matches[2].': '.$value.';
    ');
  },
  '/(-[a-zA-Z]*-)?opacity\s*:([^;]*);/im' => function($matches) {
    $value = clean_property_value($matches[2]);
    return trim('
      -ms-filter: "progid:DXImageTransform.Microsoft.Alpha(Opacity='.(floatval($value)*100).')";
      filter: alpha(opacity='.(floatval($value)*100).');
      -moz-opacity: '.$value.';
      -khtml-opacity: '.$value.';
      opacity: '.$value.';
    ');
  }
  
);

function clean_property_value($value) {
  $value = preg_replace('/\s+/im', ' ', $value);
  return trim($value);
}
