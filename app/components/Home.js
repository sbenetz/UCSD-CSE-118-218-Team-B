import { Text, ScrollView, View, ImageBackground, StyleSheet} from 'react-native';
import { FAB } from "react-native-paper";
import { useNavigation } from '@react-navigation/native';

const Home = () => {
  const navigation = useNavigation();
  
  return (
    <View style={styles.container} >
      <ImageBackground
        source = {require('../assets/add_bar_background.jpg')}
        resizeMode = "cover"
        imageStyle={{borderRadius: 10}}
        style = {styles.imageBackground}
      >
        <FAB
          style={styles.fab}
          small
          icon="plus"
          onPress={() => navigation.navigate('AddPlant')}
        />

      </ImageBackground>

      <Text style={{fontSize: 28, fontWeight: 'bold', textAlign:'left'}}>My Plants</Text>

    </View>
  )
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    // alignItems: 'center',
    justifyContent: 'center',
    backgroundColor: '#ecf0f1',
    padding: 20,
  },

  // contentContainer:{ 
  //   justifyContent: 'center',
  //   padding: 20,
  // },

  imageBackground: {
    height: 60,
    width: '100%',
    justifyContent: "center",
    borderRadius: 10,
    alignItems: 'center',
    marginTop: -520, 
    marginBottom: 30,
    alignSelf: 'flex-start',
    backgroundColor: 'black',
  },

  fab: {
    position: 'absolute',
    margin: 16,
    alignSelf: 'center',
    backgroundColor: 'rgba(255, 255, 255, 0.8)',
  },


});

export default Home;