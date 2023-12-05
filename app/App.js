import { Text, SafeAreaView, StyleSheet } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';


// You can import supported modules from npm
import { Card } from 'react-native-paper';

// or any files within the Snack
import Home from './components/Home';
import addPlant from './components/addPlant';
import PlantProfile from './components/PlantProfile';

const Stack = createStackNavigator();

export default function App() {
  return (
    <NavigationContainer>
      <Stack.Navigator initialRouteName="Home">
        <Stack.Screen name="Home" component={Home} />
        <Stack.Screen name="AddPlant" component={addPlant} />
        <Stack.Screen name="PlantProfile" component={PlantProfile} />
      </Stack.Navigator>
    </NavigationContainer>
  );
}

// const styles = StyleSheet.create({
// });
