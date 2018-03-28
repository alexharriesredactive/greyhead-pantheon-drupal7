<?php

/**
 * @file
 * conf-s3fs.php
 *
 * S3FS overrides.
 */

/**
 * Prevent S3FS trying to use S3 for all public:// or private:// files to
 * prevent catastrophes.
 */
$conf['s3fs_use_s3_for_public'] = FALSE;
$conf['s3fs_use_s3_for_private'] = FALSE;
$conf['greyhead_configuration']['overridden_variables'][] = 's3fs_use_s3_for_public';
$conf['greyhead_configuration']['overridden_variables'][] = 's3fs_use_s3_for_private';
