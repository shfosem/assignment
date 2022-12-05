package org.androidtown.homework3;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.ContentResolver;
import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.provider.MediaStore;
import android.view.View;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.RemoteViews;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.NotificationCompat;

import java.text.DateFormat;
import java.util.ArrayList;

@RequiresApi(api = Build.VERSION_CODES.O)
public class PlayMusicActivity extends AppCompatActivity implements View.OnClickListener {

    ArrayList<MusicItem> list;
    MediaPlayer mediaPlayer;
    TextView title, durationText, currentText;
    ImageView album,rewind,play,pause,forward;
    ProgressBar progressBar;
    boolean isPlaying=true;
    ContentResolver res;
    int position;
    ProgressUpdate progressUpdate;

    String CHANNEL_ID = "Channel_One";
    String name = "Title"; final int NOTIFICATION_ID=1;

    Notification notification;
    NotificationCompat.Builder builder;
    NotificationManager notificationManager;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_play);
        Intent intent = getIntent();
        mediaPlayer = new MediaPlayer();
        title = (TextView)findViewById(R.id.titleText);
        durationText = (TextView)findViewById(R.id.durationText);
        currentText =(TextView)findViewById(R.id.currentText);
        album = (ImageView) findViewById(R.id.albumImg);
        progressBar = (ProgressBar) findViewById(R.id.duration);
        position = intent.getIntExtra("position", 0);

        list = (ArrayList<MusicItem>) intent.getSerializableExtra("playlist");
        res=getContentResolver();

        rewind = (ImageView) findViewById(R.id.rewindBtn);
        play = (ImageView) findViewById(R.id.playBtn);
        forward =(ImageView) findViewById(R.id.forwardBtn);

        rewind.setOnClickListener(this);
        play.setOnClickListener(this);
        forward.setOnClickListener(this);

        playMusic(list.get(position));
        progressUpdate = new ProgressUpdate();
        progressUpdate.start();

        mediaPlayer.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mediaPlayer) {
                if(position+1<list.size()){
                    position++;
                    playMusic(list.get(position));
                }
            }
        });


    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        isPlaying=false;
        if(mediaPlayer!=null){
            mediaPlayer.release();
            mediaPlayer=null;
        }
        notificationManager.cancel(1);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.playBtn:
             /*   MusicApplication.getInstance().getServiceInterface().playBtn();
                if(MusicApplication.getInstance().getServiceInterface().isPlaying()){
                    play.setImageResource(R.drawable.play);
                }else{
                    play.setImageResource(R.drawable.pause);
                }*/

                if(isPlaying) { //pause
                    mediaPlayer.pause();
                    play.setImageResource(R.drawable.play);
                    isPlaying=false;

                }
                else {
                    mediaPlayer.seekTo(mediaPlayer.getCurrentPosition());
                    mediaPlayer.start();
                    play.setImageResource(R.drawable.pause);
                    isPlaying=true;

                }
                updateNotification();
                break;
        }
    }

    public void playMusic(MusicItem musicItem){
        try{
            progressBar.setProgress(0);
            title.setText(musicItem.getTitle());

            Uri musicUri = Uri.withAppendedPath(
                    MediaStore.Audio.Media.EXTERNAL_CONTENT_URI, String.valueOf(musicItem.getMusicId())
            );
            mediaPlayer.reset();
            mediaPlayer.setDataSource(this,musicUri);
            mediaPlayer.prepare();
            mediaPlayer.start();
            progressBar.setMax(mediaPlayer.getDuration());

            // 재생시간 표시하는 부분..
            int end, minute=0;
            end=mediaPlayer.getDuration()/1000;
            while (end>60){
                minute++;
                end-=60;
            }

          durationText.setText(end < 10 ?
                    String.valueOf(minute)+":0" + String.valueOf(end) : String.valueOf(minute)+":" + String.valueOf(end));

            /*
            String durationStr = "";
            durationText.setText(durationCovert(mediaPlayer.getDuration(),durationStr));
            */
            if(mediaPlayer.isPlaying()){
                play.setImageResource(R.drawable.pause);
            }else{
                play.setImageResource(R.drawable.play);
            }

            album.setImageBitmap(MusicAdapter.getMusicImage(getApplicationContext(),(int)musicItem.getAlbumId(),170));
            generateNotification(getApplicationContext(), musicItem);
        }catch (Exception e){
        }
    }

    public void generateNotification(Context context, MusicItem musicItem){
        NotificationChannel channel = new NotificationChannel("Channel_One", "title", NotificationManager.IMPORTANCE_DEFAULT);
        notificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.createNotificationChannel(channel);

        Intent notificationIntent = new Intent(this, PlayMusicActivity.class);
        notificationIntent.setAction(Intent.ACTION_PICK_ACTIVITY);
        notificationIntent.setFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
        PendingIntent contentIntent = PendingIntent.getActivity(context,0,notificationIntent,PendingIntent.FLAG_UPDATE_CURRENT);

        RemoteViews mRemoteViews = createRemoteView(R.layout.activity_notification, musicItem);

        builder = new NotificationCompat.Builder(context, "Channel_One");
        builder.setContentIntent(contentIntent);
        builder.setSmallIcon(R.drawable.pause);
        builder.setWhen(0);
        builder.setAutoCancel(false);
        builder.setCustomContentView(mRemoteViews);
      /*  builder.setLargeIcon(MusicAdapter.getMusicImage(getApplicationContext(), (int) musicItem.getAlbumId(), 60));
        builder.setTicker(musicItem.getTitle());
        builder.setContentTitle(musicItem.getTitle());
*/
        notification = builder.build();

        notificationManager.notify(1,notification);
    }

    private RemoteViews createRemoteView(int layoutId, MusicItem musicItem){
        RemoteViews remoteViews = new RemoteViews(this.getPackageName(), layoutId);
        remoteViews.setTextViewText(R.id.notificationTitle, musicItem.getTitle());
        remoteViews.setImageViewBitmap(R.id.notificationImg,MusicAdapter.getMusicImage(getApplicationContext(),(int)musicItem.getAlbumId(),60));
        return remoteViews;
    }
    public void updateNotification(){
        if(isPlaying) {
            builder.setSmallIcon(R.drawable.pause).build();
            notification = builder.build();
        }
        else {
            builder.setSmallIcon(R.drawable.play).build();
            notification = builder.build();
        }
        notificationManager.notify(1,notification);
    }


    public String durationCovert(long duration, String durationString){

        int minute=0;
        int second = (int) (duration/1000);
        while(second>=60){
            minute++;
            second-=60;
        }
        durationString.concat(second > 9 ?
                minute + ":" + second :
                minute + ":0" + second );

        return durationString;
    }

    private static String getCoverArtPath(long albumId, Context context){
        Uri uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;

        Cursor cursor = context.getContentResolver().query(
                uri,
                new String[]{MediaStore.Audio.Albums.ALBUM_ART},
                MediaStore.Audio.Albums._ID + " = ?",
                new String[] {String.valueOf(albumId)},
                null
        );
        boolean queryResult = cursor.moveToFirst();
        String result =null;
        if(queryResult){
            result=cursor.getString(0);
        }
        cursor.close();
        return result;
    }

    class ProgressUpdate extends Thread{
        @Override
        public void run() {
            while (true) {
                try{
                    Thread.sleep(500);
                }catch (Exception e){}
                while (isPlaying) {
                    try {
                        Thread.sleep(1000);
                        if (mediaPlayer != null) {
                            progressBar.setProgress(mediaPlayer.getCurrentPosition());
                            currentText.setText("" + mediaPlayer.getCurrentPosition() / 60000 + ":" + String.format("%02d", mediaPlayer.getCurrentPosition() / 1000 % 60));
                        }
                    } catch (Exception e) {

                    }
                }
            }
        }
    }

}


