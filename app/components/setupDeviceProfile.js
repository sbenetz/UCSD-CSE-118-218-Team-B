import React, { useMemo, useState, useEffect } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, FlatList, TextInput, Switch, TouchableOpacity, StyleSheet, Platform, Keyboard, Pressable, TouchableWithoutFeedback } from 'react-native';

const DismissKeyboard = ({ children }) => (
    <TouchableWithoutFeedback onPress={() => Keyboard.dismiss()}>
        {children}
    </TouchableWithoutFeedback>
);

function stringToHex(str) {
    let hex = '';
    for (let i = 0; i < str.length; i++) {
        const charCode = str.charCodeAt(i);
        const hexValue = charCode.toString(16);

        // Pad with zeros to ensure two-digit representation
        hex += hexValue.padStart(2, '0');
    }
    return hex;
};

const computeDevicePropHEX = (user_id, plant_type, plant_size, plant_name) => {
    // define a function that converts a string to hex
    let str = "{\"user_id\":\"" + user_id + "\", \"plant_name\":\"" + plant_name + "\",\"plant_type\":" + plant_type.toString() + ",\"plant_size\":" + plant_size.toString() + "}"
    return stringToHex(str);
}
const DeviceProps = ({ route, navigation }) => {
    const { user_id, plant_type, plant_size, plant_name } = route.params
    return (
        <DismissKeyboard>
            <KeyboardAvoidingView
                behavior={Platform.OS === 'ios' ? 'padding' : "height"}
                style={styles.container}>
                <ScrollView>

                    <Text style={{ padding: 10, fontSize: 16, fontWeight: "bold" }}>Paste the following into the LightBlue app similar to Wifi Setup:</Text>

                    <Text style={{ padding: 10, fontSize: 16, color: "blue" }} selectable={true}>
                        {computeDevicePropHEX(user_id.trim(), plant_type, plant_size, plant_name.trim())}
                    </Text>
                    <Pressable style={styles.button} onPress={() => navigation.navigate('Home', { userId: user_id })}>
                        <Text style={{ fontSize: 16, fontWeight: "bold", color: "white" }}>{"Done"}</Text>
                    </Pressable>
                </ScrollView>
            </KeyboardAvoidingView >
        </DismissKeyboard>
    )
};

const styles = StyleSheet.create({
    container: {
        flex: 1,
        paddingTop: 22,
    },
    input: {
        height: 40,
        margin: 12,
        borderWidth: 1,
        padding: 10,
    },
    button: {
        backgroundColor: '#00aeef',
        alignItems: 'center',
        justifyContent: 'center',
        paddingVertical: 12,
        paddingHorizontal: 12,
        borderRadius: 20,
    }
});
export default DeviceProps;