{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "add.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
