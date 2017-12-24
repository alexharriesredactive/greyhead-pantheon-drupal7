module.exports = function (grunt) {
  require('jit-grunt')(grunt);

  grunt.initConfig({
    bless: {
      css: {
        options: {
          // Task-specific options go here.
          banner: '/* Processed by Bless CSS. */'
        },
        files: {
          // Target-specific file lists and/or options go here.
          'css/subsubtheme-styles-split.css': 'css/subsubtheme-styles.css'
        }
      }
    },
    less: {
      development: {
        options: {
          // minify css in prod mode
          cleancss: false,
          compress: false,
          yuicompress: false,
          optimization: 0
        },
        files: {
          "css/subsubtheme-styles.css": "less/subsubtheme-styles.less" // destination file and source file
        }
      },
      //production: {
      //  options: {
      //    // minify css in prod mode
      //    cleancss: true,
      //    compress: false,
      //    yuicompress: false,
      //    optimization: 0
      //  },
      //  files: {
      //    "css/subsubtheme-styles.css": "less/subsubtheme-styles.less" // destination file and source file
      //  }
      //}
    },
    watch: {
      styles: {
        files: ['less/**/*.less'], // which files to watch
        tasks: ['less', 'bless'],
        options: {
          nospawn: true
        }
      }
    }
  });

  grunt.loadNpmTasks('grunt-bless');
  grunt.registerTask('default', ['less', 'bless', 'watch']);
};
