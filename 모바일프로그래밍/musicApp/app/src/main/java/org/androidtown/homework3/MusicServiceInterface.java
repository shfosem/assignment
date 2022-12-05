package org.androidtown.homework3;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;

import java.util.ArrayList;

public class MusicServiceInterface {
    private ServiceConnection mServiceConnection;
    private MusicService mService;

    public MusicServiceInterface(Context context){
        mServiceConnection=new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder iBinder) {
                mService =((MusicService.MusicServiceBinder)iBinder ).getService();
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                mServiceConnection=null;
                mService=null;
            }
        };
        context.bindService(new Intent(context,MusicService.class)
                .setPackage(context.getPackageName()), mServiceConnection, Context.BIND_AUTO_CREATE);
    }

    public void setMusicList(ArrayList<Long> musicIds){
        if(mService !=null){
            mService.setPlayList(musicIds);
        }
    }

    public void play(int position){
        if(mService!=null){
            mService.play(position);
        }
    }
    public  void pause(){
        if(mService!=null){
            mService.pause();
        }
    }

    public void playBtn(){
        if(isPlaying()){
            mService.pause();
        }
        else{
            mService.play();
        }
    }

    public boolean isPlaying() {
        if(mService!=null){
            return mService.isPlaying();
        }
        return false;
    }

    public MusicItem getMusicItem(){
        if(mService!=null){
            return mService.getMusicItem();
        }
        return null;
    }

}
