package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.util.Log;
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

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // test stuff here


        // stop testing stuff here


        FirebaseDatabase database = FirebaseDatabase.getInstance();
        System.out.println("!!MYLOG!! " + database);

        DatabaseReference dbRef = database.getReference();

        System.out.println("!!MYLOG!! " + dbRef);

        dbRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                DataStructure value = dataSnapshot.getValue(DataStructure.class);
                Log.d("Value", "Value: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("Value", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference ambientTempRef = dbRef.child("ambientTemp");

        ambientTempRef.addValueEventListener(new ValueEventListener() {
             @Override
             public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                 int value = dataSnapshot.getValue(int.class);
                 Log.d("ambientTemp","ambientTemp: " + value);
                 PieChartView pieChartView = findViewById(R.id.ambientTempView);
                 Log.d("piechartView", "piechartview: " + pieChartView.getChartData());
                 List<SliceValue> pieData = new ArrayList<>();
                 pieData.add(new SliceValue(value * 2).setColor(Color.RED).setLabel("" + value + "°C"));
                 pieData.add(new SliceValue(100-value).setLabel(""));
                 PieChartData pieChartData = new PieChartData(pieData);
                 pieChartData.setHasLabels(true);
                 pieChartData.setHasCenterCircle(true).setCenterText1("Ambient").setCenterText1FontSize(10).setCenterText2("temperature").setCenterText2FontSize(10);
                 pieChartView.setPieChartData(pieChartData);
             }

             @Override
             public void onCancelled(@NonNull DatabaseError databaseError) {
                 Log.w("ambientTemp", "Failed to read value with error: ", databaseError.toException());
             }
         });

        DatabaseReference fanRPMRef = dbRef.child("fanRPM");

        fanRPMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("fanRPMRef","fanRPMRef: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("fanRPMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference insideHUMRef = dbRef.child("insideHum");

        insideHUMRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("insideHUMRef","insideHUMRef: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideHUMRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference insideTempRef = dbRef.child("insideTemp");

        insideTempRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("insideTempRef","insideTempRef: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("insideTempRef", "Failed to read value with error: ", databaseError.toException());
            }
        });

        DatabaseReference ambientHumRef = dbRef.child("ambientHum");

        ambientHumRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                int value = dataSnapshot.getValue(int.class);
                Log.d("ambientHumRef","ambientHumRef: " + value);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {
                Log.w("ambientHumRef", "Failed to read value with error: ", databaseError.toException());
            }
        });
    }
}
