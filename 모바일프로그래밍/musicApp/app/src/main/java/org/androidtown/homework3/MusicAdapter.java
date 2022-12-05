package org.androidtown.homework3;

import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.ParcelFileDescriptor;
import android.provider.MediaStore;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;

public class MusicAdapter extends BaseAdapter {
    ArrayList<MusicItem> mList;
    LayoutInflater inflater;
    Activity activity;

    public MusicAdapter(Activity activity,ArrayList<MusicItem> mList){
        this.mList=mList;
        this.activity = activity;
        inflater = (LayoutInflater) activity.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return mList.size();
    }

    @Override
    public Object getItem(int i) {
        return (MusicItem)mList.get(i);
    }

    @Override
    public long getItemId(int i) {
        return i;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {
        if(view==null){
            view = inflater.inflate(R.layout.music_item,viewGroup, false);
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(
                    ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT
            );
            view.setLayoutParams(layoutParams);
        }

        ImageView imageView = (ImageView)view.findViewById(R.id.musicImage);
        Bitmap musicImg = getMusicImage(activity, (int)mList.get(i).getAlbumId(), 120);
        imageView.setImageBitmap(musicImg);

        TextView title = (TextView) view.findViewById(R.id.musicTitle);
        title.setText(mList.get(i).getTitle());

        return view;
    }

    private static  final BitmapFactory.Options options=new BitmapFactory.Options();
    public static Bitmap getMusicImage(Context context, int albumId, int MAX_IMAGE_SIZE) {
        ContentResolver resolver = context.getContentResolver();

        Uri uri = Uri.parse("content://media/external/audio/albumart/" + albumId);
        if (uri != null) {
            ParcelFileDescriptor fd = null;
            try {
                fd = resolver.openFileDescriptor(uri, "r");
                options.inJustDecodeBounds = true;
                BitmapFactory.decodeFileDescriptor(
                        fd.getFileDescriptor(), null, options
                );
                int scale = 0;
                if (options.outHeight > MAX_IMAGE_SIZE || options.outWidth > MAX_IMAGE_SIZE) {
                    scale = (int) Math.pow(2, (int) Math.round(
                            Math.log(MAX_IMAGE_SIZE / (double) Math.max(
                                    options.outHeight, options.outWidth)) / Math.log(0.5)));
                }
                options.inJustDecodeBounds = false;
                options.inSampleSize = scale;

                Bitmap bitmap = BitmapFactory.decodeFileDescriptor(
                        fd.getFileDescriptor(), null, options
                );
                if (bitmap != null) {
                    if (options.outWidth != MAX_IMAGE_SIZE || options.outHeight != MAX_IMAGE_SIZE) {
                        Bitmap temp = Bitmap.createScaledBitmap(bitmap, MAX_IMAGE_SIZE, MAX_IMAGE_SIZE, true);
                        bitmap.recycle();
                        bitmap = temp;
                    }
                }
                return bitmap;
            } catch (FileNotFoundException ex) {
            } finally {
                try {
                    if (fd != null) {
                        fd.close();
                    }
                } catch (IOException ex) {
                }
            }
        }
        Resources res = context.getResources();
        return BitmapFactory.decodeResource(res,R.drawable.default_music);
    }
}
