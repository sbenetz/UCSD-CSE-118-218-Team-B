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
const computeWiFiHEX = (ssid, password) => {
    if (ssid == "" && password == "") return "";
    // define a function that converts a string to hex
    let str = "{\"ssid\":\"" + ssid + "\", \"password\":\"" + password + "\"}"
    return stringToHex(str);
}
const EnterWifi = ({ route, navigation }) => {
    const { user_id } = route.params;
    //const [hex_output, changeHex] = useState('');
    const [wifi_ssid, setSSID] = useState('');
    const [password, setPassword] = useState('');
    return (
        <DismissKeyboard>

            <KeyboardAvoidingView
                behavior={Platform.OS === 'ios' ? 'padding' : "height"}
                style={styles.container}>
                <ScrollView>
                    <Text style={{ padding: 10, fontSize: 16 }}>
                        <Text style={{ padding: 10, fontSize: 16, fontWeight: "bold" }}>Steps to Connect:</Text>
                        {"\n1. Download and open the LightBlue app\n" +
                            "2. Press the reset button on the device\n" +
                            "3. Scroll and select \"Plant Waterer\" -> properties -> write new value\n" +
                            "4. Copy and paste the below hexidecimal value and send\n" +
                            "5. Wait for 5 blinking lights to validate connection\n" +
                            "6. Press done when you see the flashing LED."}
                    </Text>
                    <TextInput
                        style={styles.input}
                        onChangeText={setSSID}
                        placeholder="2.4 Ghz WiFi SSID"
                        value={wifi_ssid}
                    />
                    <TextInput
                        style={styles.input}
                        onChangeText={setPassword}
                        value={password}
                        placeholder="Password"
                        keyboardType="visible-password"
                        autoCapitalize="none"
                    />
                    <Text style={{ padding: 10, fontSize: 16, color: "blue" }} selectable={true}>
                        {computeWiFiHEX(wifi_ssid.trim(), password.trim())}
                    </Text>
                    <Pressable style={styles.button} onPress={() => navigation.navigate('AddPlant', {
                        user_id: user_id
                    })}>
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
export default EnterWifi;