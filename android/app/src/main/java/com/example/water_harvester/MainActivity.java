package com.example.water_harvester;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

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
                TextView textField = findViewById(R.id.text1);
                textField.setText(String.valueOf(value.getAmbientTemp()));
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
    }
}
