package com.example.station_meteo;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends Activity {
    DatabaseReference dht11Ref, dht22Ref, bmpRef; // ref bmp and dht
    TextView tempDHT11, humDHT11, tempDHT22, humDHT22, tempBMP, pressureBMP;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // ref bmp and dht
        dht11Ref = FirebaseDatabase.getInstance().getReference().child("dht11");
        dht22Ref = FirebaseDatabase.getInstance().getReference().child("dht22");
        bmpRef = FirebaseDatabase.getInstance().getReference().child("bmp280");

        // Init txt vw temp and hum dht
        tempDHT11 = findViewById(R.id.tempDHT11);
        humDHT11 = findViewById(R.id.humDHT11);

        // Init txt vw temp and hum dht22
        tempDHT22 = findViewById(R.id.tempDHT22);
        humDHT22 = findViewById(R.id.humDHT22);

        // Init txt vw prssr and temp bmp
        tempBMP = findViewById(R.id.tempBMP);
        pressureBMP = findViewById(R.id.pressionBMP);

        // dht11 listener
        dht11Ref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                DataSnapshot temperatureSnapshot = dataSnapshot.child("temperature");
                DataSnapshot humiditySnapshot = dataSnapshot.child("humidity");

                String tempdata = temperatureSnapshot.getValue().toString();
                String humdata = humiditySnapshot.getValue().toString();
                tempDHT11.setText(tempdata);
                humDHT11.setText(humdata);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                Log.e("Firebase", "Failed to read value for DHT11.", error.toException());
            }
        });

        // dht22 listener
        dht22Ref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                DataSnapshot temperatureSnapshot = dataSnapshot.child("temperature");
                DataSnapshot humiditySnapshot = dataSnapshot.child("humidity");

                String tempdata = temperatureSnapshot.getValue().toString();
                String humdata = humiditySnapshot.getValue().toString();
                tempDHT22.setText(tempdata);
                humDHT22.setText(humdata);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                Log.e("Firebase", "Failed to read value for DHT22.", error.toException());
            }
        });

        // bmp listener
        bmpRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                DataSnapshot temperatureSnapshot = dataSnapshot.child("temperature");
                DataSnapshot pressureSnapshot = dataSnapshot.child("pressure");

                String tempdata = temperatureSnapshot.getValue().toString();
                String pressuredata = pressureSnapshot.getValue().toString();
                tempBMP.setText(tempdata);
                pressureBMP.setText(pressuredata);
            }

            @Override
            public void onCancelled(DatabaseError error) {
                Log.e("Firebase", "Failed to read value for BMP280.", error.toException());
            }
        });
    }
}
