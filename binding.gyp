{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "src/observe.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
      ],
      "libraries": [

      ]
    }
  ]
}
