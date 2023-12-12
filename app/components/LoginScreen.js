import React, { useState } from 'react';
import { View, Text, TextInput, Button, StyleSheet, TouchableWithoutFeedback, Keyboard } from 'react-native';
import axios from 'axios';

const LoginScreen = ({ navigation }) => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');
    const [valid, setValid] = useState(true);
    const handleLogin = () => {
        axios.post('https://fit-glowworm-promptly.ngrok-free.app/user/login', { username, password })
            .then(response => {
                const userId = response.data.userId;
                if (userId != null) {
                    setValid(true);
                    console.log('Login successful', response.data);
                    navigation.replace('Home', { userId: userId });

                }
                else {
                    setValid(false);
                    console.log("Login Credentials Invalid")
                }

            })
            .catch(error => {
                // Handle login error
                console.error('Login error', error);
            });
    };



    return (
        <TouchableWithoutFeedback onPress={Keyboard.dismiss} accessible={false}>
            <View style={styles.container}>
                <TextInput
                    placeholder="Username"
                    value={username}
                    onChangeText={setUsername}
                    style={styles.input}
                />

                <TextInput
                    placeholder="Password"
                    value={password}
                    onChangeText={setPassword}
                    style={styles.input}
                    secureTextEntry
                />
                {!valid && <Text style={{ color: "red" }}>Login Credentials Invalid. Please try again.</Text>}
                <Button title="Login" onPress={handleLogin} />
                <Text onPress={() => navigation.navigate('Signup')}>Don't have an account? Sign up</Text>
            </View>
        </TouchableWithoutFeedback>
    );
};

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        padding: 20,
    },
    input: {
        width: '100%',
        marginVertical: 10,
        padding: 15,
        borderWidth: 1,
        borderColor: '#ddd',
        borderRadius: 5,
    },
});

export default LoginScreen;