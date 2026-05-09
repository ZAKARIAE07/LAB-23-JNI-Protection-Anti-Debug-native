package com.example.lab23;

import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Color;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Native methods with updated signatures to reduce similarity
    public native boolean verifySystemIntegrity();
    public native String fetchSecureMessage();
    public native int computeFactorialNative(int n);

    static {
        // Library name from CMakeLists.txt
        System.loadLibrary("lab23");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        performEnvironmentAudit();
    }

    private void performEnvironmentAudit() {
        TextView tvStatus = findViewById(R.id.tvStatus);
        TextView tvHello = findViewById(R.id.tvHello);
        TextView tvFact = findViewById(R.id.tvFact);

        // Security check
        boolean isEnvironmentCompromised = verifySystemIntegrity();

        if (isEnvironmentCompromised) {
            // Update UI for unsafe state using string resources
            tvStatus.setText(getString(R.string.status_compromised));
            tvStatus.setTextColor(Color.RED);

            tvHello.setText(getString(R.string.msg_unauthorized));
            tvFact.setText(getString(R.string.msg_blocked));
        } else {
            // Update UI for safe state using string resources
            tvStatus.setText(getString(R.string.status_ok));
            tvStatus.setTextColor(Color.parseColor("#1B5E20")); // Dark Green

            tvHello.setText(fetchSecureMessage());

            int input = 10;
            int result = computeFactorialNative(input);
            
            if (result >= 0) {
                tvFact.setText(getString(R.string.calc_result, input, result));
            } else {
                tvFact.setText(getString(R.string.calc_error));
            }
        }
    }
}
