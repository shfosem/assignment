package org.androidtown.ui.MobileProgramming.hw2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;


import java.util.ArrayList;
import java.util.LinkedList;

public class MyCalculator extends AppCompatActivity implements View.OnClickListener {

    private TextView calculateText;

    private Button btn0;
    private Button btn1;
    private Button btn2;
    private Button btn3;
    private Button btn4;
    private Button btn5;
    private Button btn6;
    private Button btn7;
    private Button btn8;
    private Button btn9;

    private Button btnPlus;
    private Button btnMinus;
    private Button btnMultiply;
    private Button btnDivide;

    private Button btnPoint;

    private Button btnCalculate;

    private boolean correct = false;
    private boolean getResult = false;
    private String postResult;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.calculator);
        calculateText = (TextView) findViewById(R.id.calculateText);

        //Register Buttons
        btn0 = (Button) findViewById(R.id.btn0);
        btn1 = (Button) findViewById(R.id.btn1);
        btn2 = (Button) findViewById(R.id.btn2);
        btn3 = (Button) findViewById(R.id.btn3);
        btn4 = (Button) findViewById(R.id.btn4);
        btn5 = (Button) findViewById(R.id.btn5);
        btn6 = (Button) findViewById(R.id.btn6);
        btn7 = (Button) findViewById(R.id.btn7);
        btn8 = (Button) findViewById(R.id.btn8);
        btn9 = (Button) findViewById(R.id.btn9);

        btnPlus=(Button)findViewById(R.id.btnPlus);
        btnMinus=(Button)findViewById(R.id.btnMinus);
        btnMultiply=(Button)findViewById(R.id.btnMultiply);
        btnDivide=(Button)findViewById(R.id.btnDivide);
        btnPoint=(Button)findViewById(R.id.btnPoint);

        btnCalculate=(Button)findViewById(R.id.btnCalculate);


        //Register ClickListener
        btn0.setOnClickListener(this);
        btn1.setOnClickListener(this);
        btn2.setOnClickListener(this);
        btn3.setOnClickListener(this);
        btn4.setOnClickListener(this);
        btn5.setOnClickListener(this);
        btn6.setOnClickListener(this);
        btn7.setOnClickListener(this);
        btn8.setOnClickListener(this);
        btn9.setOnClickListener(this);

        btnPlus.setOnClickListener(this);
        btnMinus.setOnClickListener(this);
        btnMultiply.setOnClickListener(this);
        btnDivide.setOnClickListener(this);
        btnPoint.setOnClickListener(this);

        btnCalculate.setOnClickListener(this);


        Button btnWebView = (Button) findViewById(R.id.btnWebActivity);
        btnWebView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                Intent intent = new Intent(getApplicationContext(),MyWebBrowser.class);
                intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);
                startActivity(intent);
            }
        });

        // Start with Intent
      /*  Intent intent =getIntent();
        String extras = intent.getStringExtra(Intent.EXTRA_TEXT);
        if(extras!=null){
            correct=true;
            calculateText.setText(extras);
            btnCalculate.performClick();

        }*/
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Intent intent = getIntent();
        onNewIntent(intent);
        setIntent(intent);
        String extras = intent.getStringExtra(Intent.EXTRA_TEXT);

        if(extras!=null){
            getResult=false;
            correct=true;
            calculateText.setText(extras);
            btnCalculate.performClick();

        }

    }

    @Override
    public void onClick(View v){

        int id = v.getId();

        Button clicked = (Button) v;

        switch (id){
            case R.id.btn0:
            case R.id.btn1:
            case R.id.btn2:
            case R.id.btn3:
            case R.id.btn4:
            case R.id.btn5:
            case R.id.btn6:
            case R.id.btn7:
            case R.id.btn8:
            case R.id.btn9:
                if(getResult){ calculateText.setText(""); getResult=false;}
                calculateText.append(clicked.getText());
                if(!correct) correct=true;
                break;

            case R.id.btnPlus:
            case R.id.btnMinus:
            case R.id.btnMultiply:
            case R.id.btnDivide:
            case R.id.btnPoint:
                if(getResult){calculateText.setText(postResult);getResult=false;}
                if(!correct){
                    calculateText.setText(calculateText.getText().subSequence(0,calculateText.length()-1));
                }
                calculateText.append(clicked.getText());
                correct=false;
                break;

            case R.id.btnCalculate:
                if(getResult) break;
                if(!correct)   {Toast.makeText(this, "잘못된 수식입니다.",Toast.LENGTH_SHORT).show(); break;}
                int result = calculate(calculateText.getText());
                calculateText.append(clicked.getText());
                postResult = Integer.toString(result);
                calculateText.append(postResult);
                getResult=true;

        }

    }

    private int calculate(CharSequence text) {
        LinkedList<Integer> numbers = new LinkedList<Integer>();
        LinkedList<Character> opList = new LinkedList<Character>();
        boolean firstNegative = false;
        //Negative Result Exception
        if(text.toString().startsWith("-")) {
            text = text.subSequence(1, text.length());
            firstNegative=true;
        }
        String num = "";
        for(int i=0; i<text.length(); i++){
            char ch = text.charAt(i);
            if(ch=='+'|| ch=='-' || ch =='*' || ch == '/' || ch=='.') {
                if(firstNegative && numbers.isEmpty()){
                    numbers.add((-1)*Integer.parseInt(num));
                    opList.add(ch);
                    num = "";
                    continue;
                }
                numbers.add(Integer.parseInt(num));
                opList.add(ch);
                num = "";
                continue;
            }
            num+=ch;
        }
        numbers.add(Integer.parseInt(num));

        while(!opList.isEmpty()){
            int pre = numbers.poll();
            int post = numbers.poll();
            char op = opList.poll();

            if(op=='+') numbers.addFirst(pre+post);
            else if(op=='-') numbers.addFirst(pre-post);
            else if(op=='*') numbers.addFirst(pre*post);
            else if(op=='/') numbers.addFirst(pre/post);
            else if(op=='.'){
                while(post>1){ post/=10;}
                numbers.addFirst(pre+post);
            }
        }
        return numbers.poll();
    }


}