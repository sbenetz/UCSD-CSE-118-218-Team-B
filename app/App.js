import { Text, StyleSheet, Button } from 'react-native';
import { AppRegistry } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';
import * as ExpoSplashScreen from 'expo-splash-screen';
import { name as appName } from './app.json';

// or any files within the Snack
import SplashScreen from './components/SplashScreen';
import LoginScreen from './components/LoginScreen';
import SignupScreen from './components/SignupScreen';
import Home from './components/Home';
import addPlant from './components/addPlant';
import PlantProfile from './components/PlantProfile';
import EnterWifi from './components/setupDeviceWifi';
import DeviceProps from './components/setupDeviceProfile';


const Stack = createStackNavigator();

// ExpoSplashScreen.preventAutoHideAsync();

// AppRegistry.registerComponent(appName, () => App);
export default function App() {
  // useEffect(() => {
  //   const prepare = async () => {
  //     await performLoadingTasksAsync();
  //     await ExpoSplashScreen.hideAsync();
  //   };
  //   prepare();
  // }, []);


  return (
    <NavigationContainer>
      <Stack.Navigator >
        {/* <Stack.Screen name="Splash" component={SplashScreen} /> */}
        <Stack.Screen name="Login" component={LoginScreen} options={{
          gestureEnabled: false, headerShown: true,
          headerLeft: () => <></>
        }} />
        <Stack.Screen name="Signup" component={SignupScreen} />
        <Stack.Screen options={({ navigation }) => ({
          gestureEnabled: false, headerShown: true,
          headerLeft: () => <></>, headerRight: () => (
            <Button
              onPress={() => navigation.navigate("Login")}
              title="Log Out"
              color="red"

            />
          ),
        })} name="Home" component={Home} />
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
