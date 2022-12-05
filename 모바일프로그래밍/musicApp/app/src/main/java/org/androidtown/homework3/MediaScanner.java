package org.androidtown.homework3;

import android.content.Context;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FilenameFilter;

public class MediaScanner{
        Context mContext;
        String mPath;

        MediaScannerConnection mMediaScanner;
        MediaScannerConnection.MediaScannerConnectionClient mMediaScannerClient;

        public static MediaScanner newInstance(Context context){
            return new MediaScanner(context);
        }

        MediaScanner(Context context){
            this.mContext = context;
        }

        public void mediaScanning(final String path){
            if(mMediaScanner == null){
                mMediaScannerClient = new MediaScannerConnection.MediaScannerConnectionClient() {
                    @Override
                    public void onMediaScannerConnected() {
                        //mMediaScanner.scanFile(mPath, "audio/mp3");
                    }
                    {
                        File file = new File(path);

                        File[] fileNames = file.listFiles(new FilenameFilter(){ // 특정 확장자만 가진 파일들을 필터링함
                            public boolean accept(File dir, String name) {
                                return name.endsWith(".mp3");
                            }
                    });
                        if(fileNames!=null){
                        for (int i = 0; i< fileNames.length; i++){
                            mMediaScanner.scanFile(fileNames[i].getAbsolutePath(), "audio/mp3");
                        }}

                    }

                    @Override
                    public void onScanCompleted(String s, Uri uri) {

                        Log.i("MediaScanner", "onScanCompleted(" + path + ", " + uri.toString() + ")");
                        /*Log.d("Success : ", "MediaScan Complete!");*/

                        mMediaScanner.disconnect();
                    }
                };
                mMediaScanner = new MediaScannerConnection(mContext, mMediaScannerClient);
            }
            mPath=path;
            mMediaScanner.connect();
        }

}