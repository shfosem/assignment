package org.androidtown.homework3;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import java.io.File;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Iterator;

public class MainActivity extends AppCompatActivity {

    private ListView listView;
    public static ArrayList<MusicItem> mList;
    MediaScannerConnection msc = null;
    MediaScannerConnection.MediaScannerConnectionClient mMscClient = null;


    @RequiresApi(api = Build.VERSION_CODES.Q)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED || checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED)
        {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE}, 0);
        }


        //Loading MusicList
        mList = getMusicList();

        listView=(ListView) findViewById(R.id.listView);
        MusicAdapter adapter = new MusicAdapter(this,mList);
        listView.setAdapter(adapter);

        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int position, long l) {
                Intent intent = new Intent(MainActivity.this, PlayMusicActivity.class);
                intent.putExtra("position", position);
                intent.putExtra("playlist", mList);
                startActivity(intent);
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();

    }
    @RequiresApi(api = Build.VERSION_CODES.Q)
    public ArrayList<MusicItem> getMusicList(){
        ArrayList<MusicItem> list = new ArrayList();
        String[] projection={
                MediaStore.Audio.Media._ID,
                MediaStore.Audio.Media.TITLE,
                MediaStore.Audio.Media.ARTIST,
                MediaStore.Audio.Media.ALBUM,
                MediaStore.Audio.Media.ALBUM_ID,
                MediaStore.Audio.Media.DURATION,
                MediaStore.Audio.Media.DATA
        };

//Environment.DIRECTORY_DOWNLOADS
        //Uri
        Uri uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
        Cursor cursor = getContentResolver().query(uri,projection,null,null,null);
        while (cursor.moveToNext()){
            MusicItem musicItem = new MusicItem();
            musicItem.setMusicId(cursor.getLong(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media._ID)));
            musicItem.setAlbumId(cursor.getLong(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.ALBUM_ID)));
            musicItem.setTitle(cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.Media.TITLE)));
            list.add(musicItem);
        }
        cursor.close();
        return list;
    }




}



 /* MediaScanner ms = MediaScanner.newInstance(MainActivity.this);

      // Toast.makeText(this, Environment.DIRECTORY_DOWNLOADS, Toast.LENGTH_SHORT).show();
        //storage/emulated/0/Download
        Uri exUri = MediaStore.Audio.Media.getContentUriForPath("storage/emulated/0/Download");
        //Toast.makeText(this, exUri.toString(), Toast.LENGTH_SHORT).show();

        // content:////media/storage/emulated/0/Download/audio/media
        //Toast.makeText(this, MediaStore.Audio.Media.EXTERNAL_CONTENT_URI.toString(), Toast.LENGTH_SHORT).show();

        // path : internal/audio/media , uri : content://media/internal/audio/media
        //Toast.makeText(this, MediaStore.Audio.Media.INTERNAL_CONTENT_URI.toString(), Toast.LENGTH_LONG).show();

        File file = new File(Environment.DIRECTORY_MUSIC);
        //Toast.makeText(this, file.getAbsolutePath(), Toast.LENGTH_SHORT).show();

        File[] fileNames = file.listFiles(new FilenameFilter() {
            @Override
            public boolean accept(File file, String name) {
                return true;
     //          return name.endsWith(".mp3");
            }
        });
        //Toast.makeText(this, fileNames[0].toString(), Toast.LENGTH_SHORT).show();

        try{
            ms.mediaScanning(
                    MediaStore.Audio.Media.getContentUri(Environment.getExternalStorageDirectory().getAbsolutePath() + "/Download").getPath()
            );
            //ms.mediaScanning(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI.getPath());

        }catch (Exception e){
            e.printStackTrace();
        }*/