import React, { useState, useEffect } from 'react';
import { KeyboardAvoidingView, ScrollView, View, Button, Image, Text, TextInput, Switch, TouchableOpacity, StyleSheet, Platform, Keyboard, TouchableWithoutFeedback } from 'react-native';
import * as ImagePicker from 'expo-image-picker';
import DropDownPicker from 'react-native-dropdown-picker';
import DateTimePicker from '@react-native-community/datetimepicker';

// const ImagePickerPlaceholder = ( { onImagePicked } ) => {
//     const [imageUrl, setImageUrl] = useState(require('../assets/image_placeholder.jpg'));


//     const pickImmage = async () => {
//         let result = await ImagePicker.launchImageLibraryAsync({
//             mediaTypes: ImagePicker.MediaTypeOptions.Images,
//             allowsEditing: Platform.OS === 'ios',
//             aspect: [4, 3],
//             quality: 1,
//         });

//         if (!result.canceled) {
//             setImageUrl({uri: result.uri});
//             if (onImagePicked) {
//                 onImagePicked(result.uri);
//             }
//         };
//     };

//     const handleImageUrlChange = (text) => {
//         setImageUrl({ uri: text });
//         if (onImagePicked) onImagePicked(text);
//     };

//     return (
//         <View style={styles.imagePickerContainer}>
//             <Button title="Upload Image" onPress={pickImmage} />

//             <TextInput
//                 style={styles.urlInput}
//                 placeholder="Or enter image URL"
//                 value={imageUrl.uri}
//                 onChangeText={handleImageUrlChange}
//                 selectTextOnFocus={true}
//             />
//             <Image source={imageUrl} style={styles.image} />


//         </View>
//     );
// }

// const DatePickerPlaceholder = ( {onDatePicked} ) => {
//     const [date, setDate] = useState(new Date());


//     const onChange = (event, selectedDate) => {
//         const currentDate = selectedDate || date;
//         setDate(currentDate);
//         if (onDatePicked) {
//             onDatePicked(currentDate);
//         }
//     };


//     return (
//         <View>
//             <DateTimePicker
//                 testID="dateTimePicker"
//                 value={date}
//                 mode="date"
//                 is24Hour={true}
//                 display="default"
//                 onChange={onChange}
//             />
//         </View>
//     );
// }

const DismissKeyboard = ({ children }) => (
    <TouchableWithoutFeedback onPress={() => Keyboard.dismiss()}>
        {children}
    </TouchableWithoutFeedback>
);

const AddPlant = ({ route, navigation }) => {
    const { user_id } = route.params;
    const [plantName, setPlantName] = useState('');
    const [plantType, setPlantType] = useState('');
    const [plantSize, setPlantSize] = useState('');
    // const [plantDate, setPlantDate] = useState('');
    // const [additionalInfo, setAdditionalInfo] = useState('');
    // const [wateringMode, setWateringMode] = useState(false);
    // const [wateringFrequency, setWateringFrequency] = useState('');
    // const [wateringAmount, setWateringAmount] = useState('');


    const [openType, setOpenType] = useState(false);
    const [types, setTypes] = useState([
        {label: 'Succulents', value: 0},
        {label: 'Flowering Plant', value: 1},
        {label: 'Herb', value: 2}
    ]);

    const [openSize, setOpenSize] = useState(false);
    const [sizes, setSizes] = useState([
        {label: 'Small', value: 0},
        {label: 'Medium', value: 1},
        {label: 'Large', value: 2}
    ]);


    const renderTags = () => {
        return sizes.map((size) => (
          <TouchableOpacity
            key={size.value}
            style={[styles.tag, plantSize === size.value && styles.selectedTag]}
            onPress={() => setPlantSize(size.value)}
          >
            <Text style={styles.tagText}>{size.label}</Text>
          </TouchableOpacity>
        ));
      };
      


    const savePlant = () => {
        // Save plant to database
        // Redirect to home page
        console.log('Plant saved!');
    };


    return (


          
        <KeyboardAvoidingView 
            behavior={Platform.OS === 'ios' ? 'padding' : "height"}
            style={styles.container}>

                <TouchableWithoutFeedback onPress={Keyboard.dismiss}>

                    <View style={styles.inputRow}>
                        <Text style={styles.header}>Name </Text>
                            <TextInput
                                style={styles.input}
                                value={plantName}
                                onChangeText={setPlantName}
                                placeholder="Enter Plant Name"
                            />
                    </View>

                    <View>
                        <View style={styles.row}>
                            <Text style={styles.header}>Plant Type</Text>
                            <DropDownPicker
                                placeholder="Select an item"
                                placeholderStyle={{ color: 'gray' }}
                                style={{ width: 260 }}
                                dropDownContainerStyle={{ width: 260 }}
                                open={openType}
                                value={plantType}
                                items={types}
                                setOpen={setOpenType}
                                setValue={setPlantType}
                                setItems={setTypes}
                                multiple={false}
                            />
                        </View>
                    </View>

                    <View style={openType ? styles.adjustedRow : null}>
                        <Text style={styles.header}>Plant Type</Text>
                        <View style={styles.tagContainer}>
                            {renderTags()}
                        </View>
                    </View>

                    <TouchableOpacity 
                        style={[styles.saveButton, openSize ? styles.adjustedRow : null]} 
                        onPress={savePlant}
                    >
                        <Text style={styles.saveButtonText}>SAVE</Text>
                    </TouchableOpacity>
                        
                

                        <View style={styles.inner}>
                            <ImagePickerPlaceholder onImagePicked={(image) => console.log(image)} />

                            <View style={styles.inputRow}>
                                <Text style={styles.header}>Name </Text>
                                <TextInput
                                    style={styles.input}
                                    value={plantName}
                                    onChangeText={setPlantName}
                                    placeholder="Enter Plant Name"
                                />
                            </View>



                            {/* <View style={styles.inputRow}>
                        <Text style={styles.header}>Date </Text>
                        <DatePickerPlaceholder onDatePicked={(date) => setPlantDate(date)} /> */}
                        </View>

                        {/* <View style={styles.row}>
                        <Text style={styles.header}>Plant Type</Text>
                        <DropDownPicker
                            open={open}
                            value={value}
                            items={items}
                            setOpen={setOpen}
                            setValue={setValue}
                            setItems={setItems}
                            multiple={false}
                        />
                    </View> */}


                        {/* <Text style={styles.header}>Additional Information</Text>
                    <TextInput
                        style={styles.additionalInfo}
                        value={additionalInfo}
                        onChangeText={setAdditionalInfo}
                        placeholder="Enter Additional Info"
                        multiline={true}
                        numberOfLines={4}
                        scrollEnabled={true}
                    /> */}

                    {/* <Text style={styles.header}>Plant Watering System</Text>
                        <View style={styles.wateringSystem}>
                            <View style={styles.row}>
                                <Text style={styles.label}>Watering Mode</Text>
                                <Switch
                                    onValueChange={(value) => setWateringMode(value)}
                                    value={wateringMode}
                                />
                            </View>

                            <View style={styles.row}>
                                <Text style={styles.label}>Watering Frequency</Text>
                            </View>

                            <View style={styles.row}>
                                <Text style={styles.label}>Watering Amount</Text>
                            </View>
                        </View> */}

                        <TouchableOpacity style={styles.saveButton} onPress={() => {
                            savePlant();
                            navigation.navigate("Set Device Properties", {
                                user_id: user_id,
                                plant_type: plantType,
                                plant_size: plantSize,
                                plant_name: plantName,
                            })
                        }}>
                            <Text style={styles.saveButtonText}>SAVE</Text>
                        </TouchableOpacity>
                    </View>

                </TouchableWithoutFeedback>
            </ScrollView>
        </KeyboardAvoidingView>
    )
};

const styles = StyleSheet.create({
    container: {
        flex: 1,
    },

    inner: {
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

    header: {
        fontWeight: 'bold',
        marginBottom: 5,
        fontSize: 15,
        marginRight: 10,
    },

    inputRow: {
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 15,
    },

    label: {
        fontSize: 12,
        fontWeight: 'bold',
        marginRight: 10,
    },

    input: {
        borderColor: '#ddd',
        backgroundColor: '#FFFFFF',
        padding: 5,
        borderRadius: 5,
        flex: 1,
    },

    row: {
        flexDirection: 'row',
        alignItems: 'center',
        marginBottom: 10,
    },

    tagContainer: {
        flexDirection: 'row', 
        justifyContent: 'space-between',
        alignItems: 'center', 
        marginBottom: 20,
    },

    tag: {
        borderWidth: 1,
        borderColor: 'grey',
        borderRadius: 20,
        padding: 10,
        margin: 5,
        alignItems: 'center',
        justifyContent: 'center',
      },

      selectedTag: {
        backgroundColor: 'lightgrey', // or any color to indicate selection
      },


    // pickerContainer: {
    //     flex: 1,
    //     height: 20,
    //     backgroundColor: '#FFFFFF',
    //     marginBottom: 10,
    //     borderRadius: 5,
    // },

    // picker: {
    //     width: '100%',
    // },

    // additionalInfo: {
    //     height: 100,
    //     textAlignVertical: 'top',
    //     backgroundColor: '#FFFFFF',
    //     borderRadius: 3,
    //     padding: 5,
    //     marginBottom: 10,
    //   },

    adjustedRow: {
        marginTop: 150,
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