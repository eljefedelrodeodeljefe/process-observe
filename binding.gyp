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
