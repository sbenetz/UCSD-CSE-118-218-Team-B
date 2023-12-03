import React, { useState } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, TextInput, TouchableOpacity, StyleSheet, Platform, Keyboard, TouchableWithoutFeedback } from 'react-native';
import * as ImagePicker from 'expo-image-picker';
import { Picker } from '@react-native-picker/picker';
import DateTimePicker from '@react-native-community/datetimepicker';

const ImagePickerPlaceholder = ( { onImagePicked } ) => {
    const [imageUrl, setImageUrl] = useState(require('../assets/image_placeholder.jpg'));

    const pickImmage = async () => {
        let result = await ImagePicker.launchImageLibraryAsync({
            mediaTypes: ImagePicker.MediaTypeOptions.Images,
            allowsEditing: Platform.OS === 'ios',
            aspect: [4, 3],
            quality: 1,
        });

        if (!result.canceled) {
            setImageUrl({uri: result.uri});
            if (onImagePicked) {
                onImagePicked(result.uri);
            }
        };
    };

    const handleImageUrlChange = (text) => {
        setImageUrl({ uri: text });
        if (onImagePicked) onImagePicked(text);
    };

    return (
        <View style={styles.imagePickerContainer}>
            <Button title="Upload Image" onPress={pickImmage} />

            <TextInput
                style={styles.urlInput}
                placeholder="Or enter image URL"
                value={imageUrl.uri}
                onChangeText={handleImageUrlChange}
                selectTextOnFocus={true}
            />
            <Image source={imageUrl} style={styles.image} />

            
        </View>
    );
}

const DatePickerPlaceholder = ( {onDatePicked} ) => {
    const [date, setDate] = useState(new Date());
    const [show, setShow] = useState(false);

    const onChange = (event, selectedDate) => {
        const currentDate = selectedDate || date;
        setShow(Platform.OS === 'ios');
        setDate(currentDate);
        if (onDatePicked) {
            onDatePicked(currentDate);
        }
    };

    const showDatepicker = () => {      
        setShow(true);
    };

    return (
        <View>
            {/* <View style={styles.datePickerContainer}>
                <Button onPress={showDatepicker} title="Select Date" />
            </View> */}
            {show && (
                <DateTimePicker
                    testID="dateTimePicker"
                    value={date}
                    mode="date"
                    is24Hour={true}
                    display="default"
                    onChange={onChange}
                />
            )}
        </View>
    );
}

const DismissKeyboard = ({ children }) => (
    <TouchableWithoutFeedback onPress={() => Keyboard.dismiss()}>
      {children}
    </TouchableWithoutFeedback>
  );

const AddPlant = ( { navigation } ) => {
    const [plantName, setPlantName] = useState('');
    const [plantDate, setPlantDate] = useState('');
    const [plantType, setPlantType] = useState('');
    const [additionalInfo, setAdditionalInfo] = useState('');

    const savePlant = () => {
        // Save plant to database
        // Redirect to home page
        console.log('Plant saved!');
    };

    return(
        <DismissKeyboard>
            <KeyboardAvoidingView>
                <ScrollView style={styles.container}>
                    {/* <Text style={styles.title}>Add a Plant</Text> */}

                    <ImagePickerPlaceholder onImagePicked={(image) => console.log(image)} />

                    <View style={styles.inputRow}>
                        <Text style={styles.label}>Name </Text>
                        <TextInput
                            style={styles.input}
                            value={plantName}
                            onChangeText={setPlantName}
                            placeholder="Enter Plant Name"
                        />
                    </View>

                    <View style={styles.inputRow}>
                        <Text style={styles.label}>Date </Text>
                        <DatePickerPlaceholder onDatePicked={(date) => setPlantDate(date)} />
                    </View>

                    <View style={styles.row}>
                        <Text style={styles.label}>Plant Type</Text>
                        <View style={styles.pickerContainer}>
                            <Picker
                                selectedValue={plantType}
                                onValueChange={(itemValue, itemIndex) => setPlantType(itemValue)}
                                style={styles.picker}
                            >
                                <Picker.Item label="Select Type" value="" />
                                <Picker.Item label="Succulents" value="succulents" />
                                <Picker.Item label="Flowering Plant" value="flowering_plant" />
                                <Picker.Item label="Foliage Plant" value="foliage_plant" />
                                <Picker.Item label="Herb" value="herb" />
                                <Picker.Item label="Fern" value="fern" />
                                <Picker.Item label="Air Plant" value="air_plant" />
                                <Picker.Item label="Cacti" value="cacti" />
                                <Picker.Item label="Palm" value="palm" />
                                <Picker.Item label="Bonsai Tree" value="bonsai_tree" />
                                <Picker.Item label="Vine and Climber" value="vine_and_climber" />
                            </Picker>
                        </View>
                    </View>


                    <Text style={styles.boldText}>Additional Information</Text>
                    <TextInput
                        style={[ styles.additionalInfo]}
                        value={additionalInfo}
                        onChangeText={setAdditionalInfo}
                        placeholder="Enter Additional Info"
                        multiline={true}
                        numberOfLines={4}
                        scrollEnabled={true}
                    />

                    <TouchableOpacity style={styles.saveButton} onPress={savePlant}>
                        <Text style={styles.saveButtonText}>SAVE</Text>
                    </TouchableOpacity>
                </ScrollView>
            </KeyboardAvoidingView>
        </DismissKeyboard>
    )
};

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: '#EDEFF2',
        padding: 20,
        paddingTop: 50,
    },

    imagePickerContainer: {
        alignItems: 'center',
        justifyContent: 'center',
        marginBottom: 30,
    },

    image: {
        width: '100%',
        height: 300,
        borderRadius: 10,
    },

    urlInput: {
        width: '100%',
        height: 25,
        backgroundColor: '#FFFFFF',
        marginBottom: 10,
        borderRadius: 5,
    },

    title: {
        fontSize: 28,
        fontWeight: 'bold',
        textAlign: 'center',
    },

    boldText: {
        fontWeight: 'bold',
        marginBottom: 5,
        fontSize: 15,
    },

    inputRow: {
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 15,
    },

    label: {
        fontSize: 15,
        fontWeight: 'bold',
        marginRight: 10,
    },

    input: {
        borderColor: '#ddd',
        backgroundColor: '#FFFFFF',
        padding: 5,
        borderRadius: 3,
        flex: 1,
    },

    row: {
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 10,
    },


    pickerContainer: {
        flex: 1,
        height: 40,
        backgroundColor: '#FFFFFF',
        marginBottom: 10,
        borderRadius: 5,
    },

    picker: {
        width: '100%',
    },

    additionalInfo: {
        height: 100,
        textAlignVertical: 'top',
        backgroundColor: '#FFFFFF',
        borderRadius: 3,
      },

    saveButton: {
        backgroundColor: '#4EBAF7',
        width: '50%',
        borderRadius: 5,
        padding: 8,
        alignItems: 'center',
        justifyContent: 'center',
        alignSelf: 'center',
        marginTop: 20,
      },

    saveButtonText: {
        color: '#fff',
        fontSize: 20,
        fontWeight: 'bold',
    },
});

export default AddPlant;