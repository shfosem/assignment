package org.androidtown.homework3;

import android.content.ContentUris;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;

import java.io.Serializable;

public class MusicItem implements Serializable {

    //static final Uri artUri = Uri.parse("content://media/external/audio/albumart");
    private long musicId;
    private long albumId;
    private String title;
    long mDuration;
    String mDataPath;
    //Uri albumArtUri;

    public MusicItem (){

    }

    public MusicItem(long id, long musicId, String title){
        this.musicId = id;
        this.albumId = musicId;
        this.title = title;
    }

    public static MusicItem bindCursor(Cursor cursor){
        MusicItem musicItem = new MusicItem();
        musicItem.musicId = cursor.getLong(cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns._ID));
        musicItem.albumId = cursor.getLong(cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.ALBUM_ID));
        musicItem.title = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.TITLE));
        musicItem.mDuration =cursor.getLong(cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.DURATION));
        musicItem.mDataPath=cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.Audio.AudioColumns.DATA));
       // musicItem.albumArtUri = ContentUris.withAppendedId(artUri, musicItem.getAlbumId());
        return musicItem;
    }

    public long getMusicId() {
        return musicId;
    }

    public void setMusicId(long musicId) {
        this.musicId = musicId;
    }

    public long getAlbumId() {
        return albumId;
    }

    public void setAlbumId(long albumId) {
        this.albumId = albumId;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }
    /* toString override? */

    @Override
    public String toString(){
        return "MusicItem{"+
                "id='" + musicId+'\''+
                ", title='"+title+'\''+
                ", artist='"+ '\''+
                '}';
    }
}
