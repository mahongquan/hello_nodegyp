{
  "targets": [
    {
	    "target_name": "hello",
	    "sources": [ "add.cc" ], 
	    "include_dirs": [
      		"<!(node -e \"require('nan')\")",
                "C:/ADLINK/PCIS-DASK/include"
            ],
		'link_settings': {

		    'libraries': [

		    'C:/ADLINK/PCIS-DASK/lib/pci-dask.lib',

		    ],

		},
    }
  ]
}
