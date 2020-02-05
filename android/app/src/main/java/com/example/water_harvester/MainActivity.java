package com.example.water_harvester;

import androidx.appcompat.app.AppCompatActivity;

import androidx.fragment.app.FragmentManager;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    private static FragmentManager fragmentManager;


    public static FragmentManager getCFragmentManager() {
        return fragmentManager;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        fragmentManager = getSupportFragmentManager();


        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new FragmentHome()).commit();




    }


}
