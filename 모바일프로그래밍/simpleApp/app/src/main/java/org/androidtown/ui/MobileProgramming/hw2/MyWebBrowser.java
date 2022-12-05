package org.androidtown.ui.MobileProgramming.hw2;

import android.content.Intent;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

public class MyWebBrowser extends AppCompatActivity {

    private WebView webView;
    private EditText urlText;
    private Button btnCalculateActivity;
    private Button btnGoBack;
    private Button btnGoForward;
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.webbrowser);




        webView=(WebView)findViewById(R.id.webView);
        WebSettings webSetting = webView.getSettings();
        webSetting.setUseWideViewPort(true);
        webSetting.setLoadWithOverviewMode(true);
        webSetting.setJavaScriptEnabled(true);
        //webSetting.setSaveFormData(false);
        //webSetting.setSupportZoom(true);



        webView.setWebViewClient(new WebViewClient(){
            @Override
            public void onPageFinished(WebView view, String url){
                urlText.setText(webView.getUrl());
            }
        });


        urlText=(EditText)findViewById(R.id.urlText);


        //WebView Movement
        urlText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                boolean handled =false;
                if(actionId == EditorInfo.IME_ACTION_GO || (keyEvent.getKeyCode()== KeyEvent.KEYCODE_ENTER && keyEvent.getAction()==KeyEvent.ACTION_DOWN)) {
                    String urlStr = urlText.getText().toString();
                    if(!urlStr.startsWith("https://")){
                        urlStr="https://"+urlStr;
                        urlText.setText(urlStr);
                    }
                    webView.loadUrl(urlStr);

                    handled=true;
                }
                return handled;
            }
        });


        //btnGoBack
        btnGoBack=(Button)findViewById(R.id.btnGoBack);
        btnGoBack.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!webView.canGoBack())
                    Toast.makeText(getApplicationContext(), "이전 페이지가 없습니다.", Toast.LENGTH_SHORT).show();
                else{
                    webView.goBack();
                }
            }
        });

        btnGoForward=(Button) findViewById(R.id.btnGoForward);
        btnGoForward.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!webView.canGoForward()){
                    Toast.makeText(getApplicationContext(), "다음 페이지가 없습니다.", Toast.LENGTH_SHORT).show();
                }
                else{
                    webView.goForward();
                }
            }
        });


        //Calculator Load
        btnCalculateActivity = (Button) findViewById(R.id.btnCalculateActivity);
        btnCalculateActivity.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(),MyCalculator.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
                startActivity(intent);
            }
        });

        // Start with Intent
        String extras = getIntent().getStringExtra(Intent.EXTRA_TEXT);
        if(extras!=null){
            if(!extras.startsWith("https://")){
                extras="https://"+extras;
                urlText.setText(extras);
            }
            webView.loadUrl(extras);
            urlText.setText(extras);

        }

    }
    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }
}
