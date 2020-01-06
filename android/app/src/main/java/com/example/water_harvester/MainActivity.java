package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.res.ResourcesCompat;

import android.graphics.Color;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import com.example.water_harvester.Classes.*;

import java.util.ArrayList;
import java.util.List;

import lecho.lib.hellocharts.model.PieChartData;
import lecho.lib.hellocharts.model.SliceValue;
import lecho.lib.hellocharts.view.PieChartView;

import static com.example.water_harvester.Classes.GraphBuilder.BuildAmbientHumPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildAmbientTempPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildFanRPMPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildInsideHumPieChartData;
import static com.example.water_harvester.Classes.GraphBuilder.BuildInsideTempPieChartData;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new FragmentFirebaseData()).commit();

        // test stuff here


        // stop testing stuff here



    }


}
