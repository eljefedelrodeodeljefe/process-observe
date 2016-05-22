{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/observe.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
      ],
      "libraries": [

      ],
      "conditions": [
        [ "OS==\"linux\" or OS==\"freebsd\" or OS==\"openbsd\" or OS==\"solaris\" or OS==\"aix\"", {
            "cflags": [
            #   "-Wall"
            ]
        }],
        [ # cflags on OS X are stupid and have to be defined like this
          "OS==\"mac\"", {
            "xcode_settings": {
            "OTHER_CFLAGS": [
              "-mmacosx-version-min=10.7",
                "-std=c++11",
                "-stdlib=libc++",
              ]
          }
        }]
      ]
    }
  ]
}
