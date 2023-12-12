import React, { useEffect } from 'react';
import { StyleSheet, View, Text } from 'react-native';
import { LinearGradient } from 'expo-linear-gradient';

const SplashScreen = () => {

  useEffect(() => {
    const loadData = async () => {
      navigation.replace('Home');
    };
    loadData();
  }, []);

    return (
        <View style={styles.container}>
            <LinearGradient
                colors={['#E9D3B3', '#88A26B']}
                start={{ x: 1, y: 0 }}
                end={{ x: 0, y: 1 }}
                style={styles.background}
            />
        </View>
    );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  background: {
    position: 'absolute',
    left: 0,
    right: 0,
    top: 0,
    bottom: 0,
  },
});

export default SplashScreen;