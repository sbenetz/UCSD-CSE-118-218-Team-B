import React, { useState } from 'react';
import { View, TextInput, Button, StyleSheet, TouchableWithoutFeedback, Keyboard } from 'react-native';
import axios from 'axios';

const SignupScreen = ({ navigation }) => {
    const [username, setUsername] = useState('');
    const [password, setPassword] = useState('');

    const handleSignup = () => {
        axios.post('https://fit-glowworm-promptly.ngrok-free.app/user/new-account', { username, password })
            .then(response => {
                console.log('Signup successful', response.data);
            })
            .catch(error => {
                console.error('Signup error', error);
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

                <Button title="Signup" onPress={handleSignup} />
                <Button title="Login" onPress={() => navigation.navigate('Login')} />
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

export default SignupScreen;