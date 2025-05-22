package org.expand.hadoop;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.DelegateToFileSystem;

public class ExpandAFS extends DelegateToFileSystem {
    ExpandAFS(final URI theUri, final Configuration conf)
            throws IOException, URISyntaxException {
        super(theUri, new Expand(), conf, "xpn", false);
    }
}
