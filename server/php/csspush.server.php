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

  include(dirname(__FILE__).'/csspush.config.inc');
  include(dirname(__FILE__).'/csspush.manipulations.inc');
  
  if(file_exists(dirname(__FILE__).'/csspush.override.inc')) {
    include(dirname(__FILE__).'/csspush.override.inc');
  }
  
  $output = array(
    'success' => false,
    'error' => '',
    'result' => null
  );
  
  // check if username/key are provided as post fields
  if(isset($_POST['username']) && isset($_POST['key']))
  {
    // check if provided user/key pair are valid
    if(check_credentials($_POST['username'], $_POST['key']))
    {
      // an array to hold possible error for every stylesheet 
      $errors = array();
      $changes = array();
      
      // loop through stylesheets and try to apply changes
      foreach($_POST['changes'] as $stylesheet)
      {
        // get css file's physical path from its url
        $stylesheet['path'] = get_physical_path($stylesheet);
          
        // check if user has permission to change this css file and also if file is writable
        if(($reason = is_changeable($stylesheet)) === true)
        {
          // apply useful manipulations on css content (e.g. cross-browsering it!)
          $content = manipulate_content($stylesheet);
          
          // write content to real css file
          $written = @file_put_contents($stylesheet['path'], $content);
          
          if($written !== false) {
            $changes[] = 'Saved file: '.$stylesheet['path'];
          } else {
            $errors[] = 'Could not write conten to file: '.$stylesheet['path'].' (reason: file_put_contents error)';
          }
        } else {
          $errors[] = 'You cannot change css file: '.$stylesheet['path'].' (reason: '.$reason.')';
        }
      }
      
      // set success flag to true since we don't exactly know whether all changes applied
      $output['success'] = true;
      $output['error'] = $errors;
      $output['result'] = $changes;
      
    } else {
      $output['error'] = 'Incorrect username/key provided.';
    }
  } else {
    $output['error'] = 'You should provide a username/key pair to use this CSSPush server.';
  }

  echo(json_encode($output));
  exit;

function check_credentials($username, $key)
{
  global $config;
  
  $valid = false;
  
  if(isset($config['users'][$username])) {
    if(is_string($config['users'][$username])) {
      $valid = ($config['users'][$username] == $key);
    } else {
      $valid = ($config['users'][$username]['key'] == $key);
    }
  }
  
  return $valid;
}

function is_changeable($stylesheet)
{
  // the variable to store current status of changeability !
  $changeable = true;
  // variable to store the reason of denial
  $reason = '';

  // check if we (as php/apache user) can write on file
  if(!is_writable($stylesheet['path'])) {
    return 'has not write permission';
  } else {
    /* check file url againts denied/allowed paths from configuration */
    
      // first we apply deny rules
        if(!isset($config['paths']['deny'])) {
          $config['paths']['deny'] = array();
        }
        if(!isset($config['users'][$_POST['username']]['deny'])) {
          $config['users'][$_POST['username']]['deny'] = array();
        }
        
        // merge global configurations with user-specific rules
        $rules = array_merge($config['paths']['deny'], $config['users'][$_POST['username']]['deny']);
        
        foreach ($rules as $rule) {
          // append case-insensitive flag if its not present in rule
          if($rule{0} !== '/') {
            $rule = '/'.$rule.'/i';
          }
          
          $result = preg_match($rule, $stylesheet['url']);
          
          if($result !== false) {
            if($result === 1) {
              $changeable = false;
              $reason = 'denial from rule '.$rule;
              // get out of the `for` and don't check other rules
              break;
            }
          } else {
            return 'rule syntax error '.$rule;
          }
        }
      
      // then we apply allow rules (only if changeable flag has been set to false)
      if(!$changeable) {
        if(!isset($config['paths']['allow'])) {
          $config['paths']['allow'] = array();
        }
        if(!isset($config['users'][$_POST['username']]['allow'])) {
          $config['users'][$_POST['username']]['allow'] = array();
        }
        
        // merge global configurations with user-specific rules
        $rules = array_merge($config['paths']['allow'], $config['users'][$_POST['username']]['allow']);
        
        foreach ($rules as $rule) {
          // append case-insensitive modifier if its not present in rule
          if($rule{0} !== '/') {
            $rule = '/'.$rule.'/im';
          }
          
          $result = preg_match($rule, $stylesheet['url']);
          
          if($result !== false) {
            if($result === 1) {
              $changeable = true;
              
              // get out of the `for` and don't check other rules
              break;
            }
          }
        }
      }
  }
  
  if($changeable) {
    return true;
  } else {
    if(empty($reason)) {
      return 'unexpected error';
    } else {
      return $reason;
    }
  }
}

function get_physical_path($stylesheet)
{
  $parsed = parse_url($stylesheet['url']);
  $path = rtrim(str_replace("\\", '/', $_SERVER['DOCUMENT_ROOT']), '/').'/'.ltrim($parsed['path'], '/');
  
  if(function_exists('override_get_physical_path')) {
    $override = override_get_physical_path($path, $stylesheet);
    
    if(!$override) {
      return $path;
    } else {
      return $override;
    }
  }
  
  return $path;
}

function manipulate_content($stylesheet)
{
  global $manipulations;
  
  $content = $stylesheet['content'];
  
  // correct missing ; preceding } so regular expression replaces
  // will not break
  $content = preg_replace_callback('/;([^;{]*)?}/im', function($matches) {
    if(trim($matches[1]) === '') {
      return '; }';
    } else {
      return ';' . $matches[1] . '; }';
    }
  }, $content);
  
  // walk through manipulations array and apply them on $content
  if(isset($manipulations) && is_array($manipulations)) {
    foreach ($manipulations as $pattern => $function) {
      $content = preg_replace_callback($pattern, $function, $content);
    }
  }
  
  // call custom user function
  if(function_exists('override_manipulate_content')) {
    $content = override_manipulate_content($content, $stylesheet);
  }
  
  return $content;
}

