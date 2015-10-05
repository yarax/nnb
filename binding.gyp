{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [ "index.cc", "functions.cc" ],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			]
        }
    ],
}