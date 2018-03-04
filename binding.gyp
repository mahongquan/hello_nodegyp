{
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "atexit.cc" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
