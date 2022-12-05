package org.androidtown.homework3;

import android.app.IntentService;
import android.app.Service;
import android.content.Intent;
import android.database.Cursor;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Binder;
import android.os.IBinder;
import android.os.PowerManager;
import android.provider.MediaStore;

import androidx.annotation.Nullable;

import java.util.ArrayList;

public class MusicService extends Service {

    final IBinder mBinder = new MusicServiceBinder();
    MediaPlayer mMediaPlayer;
    boolean isPrepared;
    int position;

    MusicItem mMusicItem;

    ArrayList<Long> mMusicList = new ArrayList<>();



    @Override
    public void onCreate() {
        super.onCreate();
        mMediaPlayer = new MediaPlayer();
        mMediaPlayer.setWakeMode(getApplicationContext(), PowerManager.PARTIAL_WAKE_LOCK);
        mMediaPlayer.setOnPreparedListener(new MediaPlayer.OnPreparedListener() {
            @Override
            public void onPrepared(MediaPlayer mediaPlayer) {
                isPrepared=true;
                mediaPlayer.start();
            }
        });

        mMediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer) {
                isPrepared = false;
            }
        });
        mMediaPlayer.setOnErrorListener(new MediaPlayer.OnErrorListener() {
            @Override
            public boolean onError(MediaPlayer mediaPlayer, int i, int i1) {
                isPrepared =false;
                return false;
            }
        });
        mMediaPlayer.setOnSeekCompleteListener(new MediaPlayer.OnSeekCompleteListener() {
            @Override
            public void onSeekComplete(MediaPlayer mediaPlayer) {

            }
        });
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }


    @Override
    public void onDestroy() {
        super.onDestroy();
        if(mMediaPlayer!=null){
            mMediaPlayer.stop();
            mMediaPlayer.release();
            mMediaPlayer=null;
        }
    }

    public class MusicServiceBinder extends Binder{
        MusicService getService(){
            return MusicService.this;
        }
    }

    public void setPlayList(ArrayList<Long> musicList) {
        if(mMusicList.size()!=musicList.size()){
            if(!mMusicList.equals(musicList)){
                mMusicList.clear();
                mMusicList.addAll(musicList);
            }
        }
    }

    private void queryMusicItem(int currentPosition){
        position = currentPosition;
        long musicId = mMusicList.get(position);
        // Uri Internal..
        Uri uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
        String[] projection = new String[]{
                MediaStore.Audio.Media._ID,
                MediaStore.Audio.Media.TITLE,
                MediaStore.Audio.Media.ARTIST,
                MediaStore.Audio.Media.ALBUM,
                MediaStore.Audio.Media.ALBUM_ID,
                MediaStore.Audio.Media.DURATION,
                MediaStore.Audio.Media.DATA
        };
        String selection = MediaStore.Audio.Media._ID + " = ?";
        String[] selectionArgs = {String.valueOf(musicId)};
        Cursor cursor = getContentResolver().query(uri,projection,selection,selectionArgs,null);
        if(cursor!=null){
            if(cursor.getCount()>0){
                cursor.moveToFirst();
                mMusicItem= MusicItem.bindCursor(cursor);
            }
            cursor.close();
        }
    }

    private void prepare(){
        try{
            mMediaPlayer.setDataSource(mMusicItem.mDataPath);
            mMediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mMediaPlayer.prepareAsync();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public void play(int currentPosition){
        queryMusicItem(currentPosition);
        stop();
        prepare();
    }
    public void play(){
        if(isPrepared){
            mMediaPlayer.start();
        }
    }
    public void pause(){
        if(isPrepared){
            mMediaPlayer.pause();
        }
    }
    public void stop(){
        mMediaPlayer.stop();
        mMediaPlayer.reset();
    }

    public void forward(){
        if(mMusicList.size() -1 > position){
            position++;
        }else{
            position=0;
        }
        play(position);
    }

    public void rewind(){
        if(position>0){
            position--;
        }else {
            position=mMusicList.size()-1;
        }
        play(position);
    }

    public MusicItem getMusicItem(){
        return mMusicItem;
    }

    public boolean isPlaying(){
        return mMediaPlayer.isPlaying();
    }
}




