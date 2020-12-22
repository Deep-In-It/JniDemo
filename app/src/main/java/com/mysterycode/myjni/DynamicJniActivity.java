package com.mysterycode.myjni;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class DynamicJniActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dynamic_jni);
        TextView tvDynamicRegister = findViewById(R.id.tvDynamicRegister);

        dynamicRegister("我是动态注册的");
        tvDynamicRegister.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dynamicRegister2("XXX");
            }
        });
    }

    //动态注册
    public native void dynamicRegister(String name);

    //因为在MainActivity中进行了声明，故此处可以省略
//    static {
//        System.loadLibrary("native-lib");
//    }

    public native void dynamicRegister2(String name);

    private void testException() throws NullPointerException{
        throw new NullPointerException("testException NullPointerException.");
    }
}