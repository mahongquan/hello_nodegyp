{
  "targets": [
    {
	    "target_name": "hello",
	    "sources": [ "wrapobject.cc","myobject.cc" ], 
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
