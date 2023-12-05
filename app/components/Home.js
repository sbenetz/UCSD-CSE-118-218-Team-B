import { Text, Image, ScrollView, View, TouchableOpacity, ImageBackground, StyleSheet, FlatList } from 'react-native';
import { FAB } from "react-native-paper";
import { useNavigation } from '@react-navigation/native';

import React, { useState, useEffect } from 'react';

const fetchPlants = async () => {
  return [
    { id: '1', name: 'Goldfish Plant', image: require('../assets/goldfish-plant.png'), health: 4 },
    { id: '2', name: 'Pathos', image: require('../assets/pathos-plant.png'), health: 5 },
    // ... more plants
  ];
};

const PlantCard = ({ plant, onPress }) => {
  const [rating, setRating] = useState(plant.health);

  const healthActiveColor = "#70A327";
  const healthInactiveColor = "rgba(112, 163, 39, 0.5)";
  const unhealthActiveColor = "#FF0120";
  const unhealthInactiveColor = "rgba(255, 1, 32, 0.5)";

  const activeColor = rating > 2 ? healthActiveColor : unhealthActiveColor;
  const inactiveColor = rating > 2 ? healthInactiveColor : unhealthInactiveColor;

  return (
    <View style={styles.card}>
      <TouchableOpacity onPress={() => onPress(plant)}>
        <Image source={plant.image} style={styles.cardImage} />
      </TouchableOpacity>

      <View style={styles.cardLabel}>
        <Text style={styles.cardTitle}>{plant.name}</Text>
      </View>
    </View>
  )
};

const Home = () => {
  const [plants, setPlants] = useState([]);
  const navigation = useNavigation();

  useEffect(() => {
    const getPlants = async () => {
      const data = await fetchPlants();
      setPlants(data);
    };
    getPlants();
  }, []);

  const goToPlantProfile = (plant) => {
    navigation.navigate('PlantProfile', { plantId: plant.id });
  }


  const renderPlant = ({ item, index }) => {
    const isEven = index % 2 === 0;
    const imageStyle = isEven ? styles.leftImage : styles.rightImage;

    return (
      <View style={imageStyle}>
        <PlantCard plant={item} onPress={goToPlantProfile} />
      </View>
    );
  };


  return (

    <View style={styles.container} >
      <ImageBackground
        source={require('../assets/add_bar_background.jpg')}
        resizeMode="cover"
        imageStyle={{ borderRadius: 10 }}
        style={styles.imageBackground}
      >
        <FAB
          style={styles.fab}
          small
          icon="plus"
          onPress={() => navigation.navigate('connectBluetooth')}
        />

      </ImageBackground>

      <Text style={{ fontSize: 28, fontWeight: 'bold', textAlign: 'left' }}>My Plants</Text>
      <FlatList
        data={plants}
        renderItem={renderPlant}
        keyExtractor={item => item.id}
        numColumns={2}
        contentContainerStyle={styles.list}
      />
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


  card: {
    flex: 1,
  },

  cardImage: {
    width: 150,
    height: 160,
    borderRadius: 10,
  },

  leftImage: {
    flex: 1,
    marginRight: 10,
    marginBottom: 10,
  },

  rightImage: {
    flex: 1,
    marginLeft: 20,
    marginBottom: 10,
  },

});

export default Home;