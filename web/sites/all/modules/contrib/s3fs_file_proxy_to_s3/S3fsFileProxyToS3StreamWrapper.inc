<?php

/**
 * The stream wrapper class.
 */
class S3fsFileProxyToS3StreamWrapper extends S3fsStreamWrapper {

  /**
   * If file doesn't exist or isn't public scheme will use our router with
   * 's3fs_to_s3/files/%' path and this will get from stage_file_proxy url the
   * necessary files.
   */
  public function getExternalUrl() {
    $s3_key = str_replace('\\', '/', file_uri_target($this->uri));
    $path_parts = explode('/', $s3_key);
    if (
      file_exists($this->uri) ||
      file_uri_scheme($this->uri) != 'public'
    ) {
      return parent::getExternalUrl();
    }
    else {
      array_unshift($path_parts, 's3fs_to_s3', 'files');
      return url(implode('/', $path_parts), array('absolute' => TRUE));
    }
  }

}
