package com.supergoon.rpg;

import org.libsdl.app.SDLActivity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;

public class EscapeTheFateActivity extends SDLActivity {

    private static final String TAG = "ETF";

    @Override
    protected String[] getLibraries() {
        return new String[]{"EscapeTheFate"};
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        extractAssets();
        super.onCreate(savedInstanceState);
    }

    private void extractAssets() {
        try {
            Context ctx = getApplicationContext();
            String destDir = ctx.getFilesDir().getAbsolutePath() + "/data/";
            File dir = new File(destDir);
            if (!dir.exists()) dir.mkdirs();

            File destFile = new File(destDir + "etf.sg");
            if (!destFile.exists()) {
                Log.i(TAG, "Extracting etf.sg to internal storage...");
                InputStream in = ctx.getAssets().open("etf.sg");
                FileOutputStream out = new FileOutputStream(destFile);
                byte[] buf = new byte[65536];
                int n;
                while ((n = in.read(buf)) > 0) out.write(buf, 0, n);
                in.close();
                out.close();
                Log.i(TAG, "etf.sg extracted successfully");
            }
        } catch (IOException e) {
            Log.e(TAG, "Failed to extract etf.sg: " + e.getMessage());
        }
    }
}
