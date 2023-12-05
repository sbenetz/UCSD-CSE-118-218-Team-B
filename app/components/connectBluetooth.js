import React, { useMemo, useState, useEffect } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, FlatList, TextInput, Switch, TouchableOpacity, StyleSheet, Platform, Keyboard, TouchableWithoutFeedback } from 'react-native';
import {
    BleError,
    BleManager,
    Characteristic,
    Device,
} from "react-native-ble-plx";

const DismissKeyboard = ({ children }) => (
    <TouchableWithoutFeedback onPress={() => Keyboard.dismiss()}>
        {children}
    </TouchableWithoutFeedback>
);

export const BLE_NAME = "SAMPlE_BLE"
export const BLE_SERVICE_ID = "5476534d-1213-1212-1212-454e544f1212"
export const BLE_READ_CHAR_ID = "00105354-0000-1000-8000-00805f9b34fb"
export const BLE_WRITE_CHAR_ID = "00105352-0000-1000-8000-00805f9b34fb"

const SelectBluetooth = ({ navigation }) => {

    const bleManager = useMemo(() => new BleManager(), []);
    // const [allDevices, setAllDevices] = useState < Device[] > ([]);
    // const [connectedDevice, setConnectedDevice] = useState < Device | null > (null);
    var UUIDs = []
    bleManager.startDeviceScan(null, null, (error, device) => {
        if (error) {
            console.log(error);
        }
        if (device && device.name) {
            UUIDs.push({ key: device.name });
        }
    });

    return (
        <KeyboardAvoidingView
            behavior={Platform.OS === 'ios' ? 'padding' : "height"}
            style={styles.container}>
            <FlatList
                data={UUIDs}
                renderItem={({ item }) => <Text style={styles.item}>{item.key}</Text>}
            />
        </KeyboardAvoidingView>
    )
};

const styles = StyleSheet.create({
    container: {
        flex: 1,
        paddingTop: 22,
    },
    item: {
        padding: 10,
        fontSize: 18,
        height: 44,
    },
});
export default SelectBluetooth;