package org.androidtown.ui.mobileprogramming.mymemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.ActionMode;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private EditText editText;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        editText=(EditText) findViewById(R.id.editText);

        editText.setCustomSelectionActionModeCallback(new ActionMode.Callback() {
            @Override
            public boolean onCreateActionMode(ActionMode actionMode, Menu menu) {
                return true;
            }

            @Override
            public boolean onPrepareActionMode(ActionMode actionMode, Menu menu) {
                return false;
            }

            @Override
            public boolean onActionItemClicked(ActionMode actionMode, MenuItem menuItem) {
                int start = editText.getSelectionStart();
                int end = editText.getSelectionEnd();

                String str = editText.getText().toString();
                str=str.substring(start,end);

                switch (menuItem.getItemId()){
                    case android.R.id.shareText:
                        boolean isCalculate = false;
                        Intent intent = new Intent();
                        intent.setAction(Intent.ACTION_SEND);
                        //when 'str' starts with number, it may be a expression
                        if(Character.isDigit(str.charAt(0)))
                            isCalculate=true;

                        if(isCalculate) {
                            /* Toast.makeText(getApplicationContext(), "share 버튼 눌렸음", Toast.LENGTH_SHORT).show();*/
                            intent.addCategory(Intent.CATEGORY_APP_CALCULATOR);
                        }
                        else{
                            intent.addCategory(Intent.CATEGORY_BROWSABLE);
                        }
                        intent.putExtra(Intent.EXTRA_TEXT, str);
                        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        intent.setType("text/plain");

                        Intent shareIntent = Intent.createChooser(intent,null);
                        startActivity(shareIntent);
                }

                return true;
            }

            @Override
            public void onDestroyActionMode(ActionMode actionMode) {

            }
        });
    }
}