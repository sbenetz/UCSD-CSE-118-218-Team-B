import { Text, SafeAreaView, StyleSheet } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';


// You can import supported modules from npm
import { Card } from 'react-native-paper';

// or any files within the Snack
import Home from './components/Home';
import addPlant from './components/addPlant';
import PlantProfile from './components/PlantProfile';
import EnterWifi from './components/setupDeviceWifi';
import DeviceProps from './components/setupDeviceProfile';

const Stack = createStackNavigator();

export default function App() {
  return (
    <NavigationContainer>
      <Stack.Navigator initialRouteName="Home">
        <Stack.Screen name="Home" component={Home} />
        <Stack.Screen name="AddPlant" component={addPlant} />
        <Stack.Screen name="PlantProfile" component={PlantProfile} />
        <Stack.Screen name="Connect Device to Wifi" component={EnterWifi} />
        <Stack.Screen name="Set Device Properties" component={DeviceProps} />
      </Stack.Navigator>
    </NavigationContainer>
  );
}

// const styles = StyleSheet.create({
// });
