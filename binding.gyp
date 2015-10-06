{
    "targets": [
        {
            "target_name": "nnb",
            "sources": [ "src/index.cc", "src/functions.cc" ],
            "include_dirs" : [
 	 			"<!(node -e \"require('nan')\")"
			]
        }
    ],
}
