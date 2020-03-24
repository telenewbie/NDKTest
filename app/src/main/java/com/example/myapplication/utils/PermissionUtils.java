package com.example.myapplication.utils;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;

public class PermissionUtils {
    public static boolean checkPermission(Activity activity, String permission, int requstCode) {
        int checkCallPhonePermission = ContextCompat.checkSelfPermission(activity, permission);
        if (checkCallPhonePermission != PackageManager.PERMISSION_GRANTED) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(activity, permission)) {
                Toast.makeText(activity, "已经拒绝该权限", Toast.LENGTH_SHORT).show();
            } else {
                ActivityCompat.requestPermissions(activity, new String[]{permission},
                        requstCode);
            }
            return false;
        } else {
            return true;
        }
    }

    public static boolean checkPermission(Fragment fragment, int requestCode, String... permissions) {
        List<String> permissionsList = new ArrayList<>();
        for (String permission : permissions) {
            if (ContextCompat.checkSelfPermission(fragment.getContext(), permission) != PackageManager.PERMISSION_GRANTED) {
                permissionsList.add(permission);
            }
        }
        if (permissionsList.size() == 0) return true;
        for (String permission : permissionsList) {
            if (fragment.shouldShowRequestPermissionRationale(permission)) {
                Toast.makeText(fragment.getContext(), "您已经拒绝该权限,请在系统设置中打开", Toast.LENGTH_SHORT).show();
                return false;
            }
        }
        fragment.requestPermissions(permissionsList.toArray(new String[permissionsList.size()]), requestCode);
        return false;
    }

    public static boolean checkPermission(Fragment fragment, String permission, int requestCode) {
        int checkCallPhonePermission = ContextCompat.checkSelfPermission(fragment.getContext(), permission);
        if (checkCallPhonePermission != PackageManager.PERMISSION_GRANTED) {
            if (fragment.shouldShowRequestPermissionRationale(permission)) {
                Toast.makeText(fragment.getContext(), "已经拒绝该权限", Toast.LENGTH_SHORT).show();
            } else {
                fragment.requestPermissions(new String[]{permission},
                        requestCode);
            }
            return false;
        } else {
            return true;
        }
    }

    public static boolean checkCanDrawOverlays(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (!Settings.canDrawOverlays(context)) {
                Intent drawOverlaysSettingsIntent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION);
                drawOverlaysSettingsIntent.setData(Uri.parse("package:" + context.getPackageName()));
                context.startActivity(drawOverlaysSettingsIntent);
                return false;
            }
        }
        return true;
    }


}