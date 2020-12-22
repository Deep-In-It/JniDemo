package com.mysterycode.myjni;

import androidx.appcompat.app.AppCompatActivity;


import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("native-lib");
    }

    private int count = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        Person p = new Person();
        p.setName("一一");
        /** 1\. Java 数据传递给 native */
        test1(true,
                (byte) 1,
                ',',
                (short) 3,
                4,
                3.3f,
                2.2d,
                "DevYK",
                28,
                new int[]{1, 2, 3, 4, 5, 6, 7},
                new String[]{"1", "2", "4"},
                p,
                new boolean[]{false, true}
        );

        String str = getPerson().toString();
        tv.setText(str);
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startActivity(new Intent(MainActivity.this, DynamicJniActivity.class));
            }
        });

        for (int i = 0; i < 100; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    Log.e(">>>>>", String.valueOf(count++));
                }
            }).start();
        }
    }

    public native Person getPerson();

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    /**
     * Java 将数据传递到 native 中
     */
    public native void test1(
            boolean b,
            byte b1,
            char c,
            short s,
            long l,
            float f,
            double d,
            String name,
            int age,
            int[] i,
            String[] strs,
            Person person,
            boolean[] bArray
    );
}
